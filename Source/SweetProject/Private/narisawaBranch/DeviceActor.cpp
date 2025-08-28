// Source/SweetProject/Private/narisawaBranch/DeviceActor.cpp

#include "narisawaBranch/DeviceActor.h"
#include "ASerialLibControllerWin.h"
#include "WindowsSerial/WindowsSerial.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "ASerialCore/ASerialPacket.h" // ASerialDataStructを使用するために必要

//----------------------------------------------------------------------
// 1. 通信スレッドの実装
//----------------------------------------------------------------------

// 通信処理を専門に行う内部クラス
class FDeviceCommunicationTask : public FRunnable
{
public:
	// コンストラクタ：必要なポインタを受け取る
	FDeviceCommunicationTask(UASerialLibControllerWin* InController, TQueue<uint8, EQueueMode::Mpsc>* InCmdQueue, TQueue<FSerialData, EQueueMode::Spsc>* InDataQueue)
		: SerialController(InController)
		, CommandQueue(InCmdQueue)
		, DataQueue(InDataQueue)
		, bIsRunning(true)
	{
	}

	// スレッドのメインループ
	virtual uint32 Run() override
	{
		while (bIsRunning)
		{
			uint8 Command;
			// 処理すべきコマンドがあれば実行
			if (CommandQueue->Dequeue(Command))
			{
				if (SerialController->WriteData(Command) == 0)
				{
					ASerialDataStruct::ASerialData ReceivedData;
					// ReadDataが成功(1)を返すまで、またはタイムアウト(-2)するまで待つ
					if (SerialController->ReadData(&ReceivedData) == 1)
					{
						// 成功したら、結果をメインスレッドに渡す
						FSerialData DataToQueue;
						DataToQueue.command = ReceivedData.command;
						DataToQueue.data.Append(ReceivedData.data, ReceivedData.data_num);
						DataQueue->Enqueue(DataToQueue);
					}
				}
			}
			else
			{
				// キューが空なら、CPUを無駄遣いしないように少し待機
				FPlatformProcess::Sleep(0.01f);
			}
		}
		return 0;
	}

	// スレッドの停止を要求
	virtual void Stop() override
	{
		bIsRunning = false;
	}

private:
	UASerialLibControllerWin* SerialController;
	TQueue<uint8, EQueueMode::Mpsc>* CommandQueue;
	TQueue<FSerialData, EQueueMode::Spsc>* DataQueue;
	FThreadSafeBool bIsRunning;
};

//----------------------------------------------------------------------
// 2. DeviceActorの実装
//----------------------------------------------------------------------

ADeviceActor::ADeviceActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsDeviceConnected = false;
	SerialController = nullptr;
	SerialInterface = nullptr;
	CommunicationTask = nullptr;
	CommunicationThread = nullptr;
}

void ADeviceActor::BeginPlay()
{
	Super::BeginPlay();

	// シリアル通信コントローラーの初期化
	SerialController = NewObject<UASerialLibControllerWin>(this);
	SerialInterface = new WindowsSerial();
	SerialController->Initialize(0x10, 0x01);
	SerialController->SetInterfacePt(SerialInterface);

	// デバイスへの接続試行
	if (SerialController->AutoConnectDevice() == ConnectResult::Succ)
	{
		bIsDeviceConnected = true; // 接続成功フラグを立てる
		UE_LOG(LogTemp, Log, TEXT("Device connected successfully."));

		// 2秒待機してデバイスの起動を待つ
		FPlatformProcess::Sleep(2.0f);

		// LED点滅コマンドを送信
		SerialController->WriteData(0x30);

		// 通信スレッドを生成して開始
		CommunicationTask = new FDeviceCommunicationTask(SerialController, &CommandQueue, &DataQueue);
		CommunicationThread = FRunnableThread::Create(CommunicationTask, TEXT("DeviceCommunicationThread"));

		// 全てのデータ取得コマンドを一度だけキューに入れる
		CommandQueue.Enqueue(0x31);
		CommandQueue.Enqueue(0x32);
		CommandQueue.Enqueue(0x33);
	}
	else
	{
		bIsDeviceConnected = false;
		UE_LOG(LogTemp, Error, TEXT("Failed to auto-connect to device."));
	}
}

void ADeviceActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// スレッドを安全に停止・破棄
	if (CommunicationThread)
	{
		CommunicationTask->Stop();
		CommunicationThread->WaitForCompletion();
		delete CommunicationThread;
		CommunicationThread = nullptr;
		delete CommunicationTask;
		CommunicationTask = nullptr;
	}

	// デバイスを切断
	if (SerialController && bIsDeviceConnected)
	{
		SerialController->DisConnectDevice();
		bIsDeviceConnected = false;
	}

	// WindowsSerialオブジェクトを破棄
	if (SerialInterface)
	{
		delete SerialInterface;
		SerialInterface = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void ADeviceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsDeviceConnected) return;

	// データキューにデータがあれば処理する
	FSerialData Data;
	while (DataQueue.Dequeue(Data))
	{
		ProcessReceivedData(Data);
	}
}

void ADeviceActor::ProcessReceivedData(const FSerialData& Data)
{
	UE_LOG(LogTemp, Log, TEXT("SUCCESS: Data received for command 0x%X, length: %d"), Data.command, Data.data.Num());

	switch (Data.command)
	{
	case 0x31: // 姿勢
		if (Data.data.Num() == 8)
		{
			FMemory::Memcpy(&DevicePoseXY, Data.data.GetData(), 8);
			UE_LOG(LogTemp, Log, TEXT("  -> Pose XY: X=%.3f, Y=%.3f"), DevicePoseXY.X, DevicePoseXY.Y);
		}
		break;
	case 0x32: // 加速度
		if (Data.data.Num() == 4)
		{
			FMemory::Memcpy(&DeviceAcceleration, Data.data.GetData(), 4);
			UE_LOG(LogTemp, Log, TEXT("  -> Acceleration: %.3f"), DeviceAcceleration);
		}
		break;
	case 0x33: // ボタン
		if (Data.data.Num() == 1)
		{
			ButtonState = static_cast<int32>(Data.data[0]);
			UE_LOG(LogTemp, Log, TEXT("  -> Button State: %d"), ButtonState);
		}
		break;
	}
}