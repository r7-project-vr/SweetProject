// Plugins/ASerialCom/Source/ASerialCom/Private/ASerialComBPLibrary.cpp の内容をこれで上書き

#include "ASerialComBPLibrary.h"
#include "ASerialCom.h"
#include "Core.h"
#include "Async/Async.h"

FOnSensorDataReceived UASerialComBPLibrary::OnSensorDataReceived;
TUniquePtr<ASerial_lib_Controller_Win> UASerialComBPLibrary::ASerialController = nullptr;
TUniquePtr<WindowsSerial> UASerialComBPLibrary::SerialInterface = nullptr;
bool UASerialComBPLibrary::bIsConnected = false;
FTickerDelegate UASerialComBPLibrary::TickDelegate;
FDelegateHandle UASerialComBPLibrary::TickHandle;


// Connect関数の引数を更新
int32 UASerialComBPLibrary::Connect(int32 DeviceId, int32 DeviceVersion)
{
    if (bIsConnected) return -1;

    // IDとバージョンでインスタンスを生成
    ASerialController = MakeUnique<ASerial_lib_Controller_Win>(DeviceId, DeviceVersion);
    SerialInterface = MakeUnique<WindowsSerial>();
    ASerialController->SetInterfacePt(SerialInterface.Get());

    const int32 ConnectedComPort = ASerialController->AutoConnectDevice();
    if (ConnectedComPort != -1)
    {
        bIsConnected = true;
        TickDelegate = FTickerDelegate::CreateStatic(&UASerialComBPLibrary::Tick);
        TickHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);
        UE_LOG(LogTemp, Log, TEXT("ASerial device connected on COM %d"), ConnectedComPort);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ASerial device connection failed."));
    }
    return ConnectedComPort;
}

// Disconnect関数 (変更なし)
void UASerialComBPLibrary::Disconnect()
{
    if (bIsConnected && ASerialController.IsValid())
    {
        FTicker::GetCoreTicker().RemoveTicker(TickHandle);
        ASerialController->DisConnectDevice();
        ASerialController.Reset();
        SerialInterface.Reset();
        bIsConnected = false;
        UE_LOG(LogTemp, Log, TEXT("ASerial device disconnected."));
    }
}

// --- ★★★ 変更点 ★★★ ---
// RequestSensorData関数の実装
void UASerialComBPLibrary::RequestSensorData()
{
    if (bIsConnected && ASerialController.IsValid())
    {
        // ご指定のコマンド0x20を送信
        ASerialController->WriteData(0x20);
    }
}

// Tick関数 (変更なし)
bool UASerialComBPLibrary::Tick(float DeltaTime)
{
    if (bIsConnected && ASerialController.IsValid())
    {
        static ASerialDataStruct::ASerialData receivedData;
        const int32 ReadStatus = ASerialController->ReadDataProcess(&receivedData);

        if (ReadStatus == 1 && receivedData.command == 0x20)
        {
            TArray<uint8> data;
            data.Append(receivedData.data, receivedData.data_num);
            AsyncTask(ENamedThreads::GameThread, [data]()
                {
                    ParseSensorData(data.GetData(), data.Num());
                });
        }
    }
    return true;
}

// --- ★★★ 変更点 ★★★ ---
// ParseSensorData関数の実装
void UASerialComBPLibrary::ParseSensorData(const uint8* data, uint8 data_num)
{
    // 回転(16) + 加速度(12) + ボタン(1) = 29バイト以上あるかチェック
    if (data_num < 29) return;

    FQuat rotationQuat;
    FVector accelerationVec;
    bool bIsPressed = false;

    // メモリコピーでバイト配列を構造体に変換
    FMemory::Memcpy(&rotationQuat, &data[0], sizeof(FQuat));
    FMemory::Memcpy(&accelerationVec, &data[16], sizeof(FVector));

    // 29バイト目をボタンの状態として解釈 (0以外ならtrue)
    bIsPressed = (data[28] != 0);

    // イベントを発行してブループリントに3つのデータを渡す
    OnSensorDataReceived.Broadcast(rotationQuat.Rotator(), accelerationVec, bIsPressed);
}