// Source/SweetProject/Public/narisawaBranch/DeviceActor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Queue.h" // TQueueを使用するために必要
#include "DeviceActor.generated.h"

// 前方宣言：コンパイル時間を短縮するため、クラス名を宣言するに留める
class UASerialLibControllerWin;
class FRunnableThread;
class WindowsSerial;

// スレッドとメインスレッド間でデータを安全に受け渡すための構造体
// ヘッダファイル内でTQueueの型として使用するため、UCLASSの前に定義する
struct FSerialData
{
	uint8 command;
	TArray<uint8> data;
};

UCLASS()
class SWEETPROJECT_API ADeviceActor : public AActor
{
	GENERATED_BODY()

public:
	ADeviceActor();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Device Data")
	FVector2D DevicePoseXY;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Device Data")
	float DeviceAcceleration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Device Data")
	int32 ButtonState;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	// 受信したデータを解析し、プロパティを更新する
	void ProcessReceivedData(const FSerialData& Data);

	// ASerialComプラグインの通信コントローラー
	UPROPERTY()
	TObjectPtr<UASerialLibControllerWin> SerialController;

	// WindowsSerial APIの実体
	WindowsSerial* SerialInterface;

	// 通信処理を行うバックグラウンドスレッド
	class FDeviceCommunicationTask* CommunicationTask; // 実装はcppファイル内
	FRunnableThread* CommunicationThread;

	// スレッドとのデータ受け渡しを行うためのキュー（データの待合室）
	TQueue<uint8, EQueueMode::Mpsc> CommandQueue;
	TQueue<FSerialData, EQueueMode::Spsc> DataQueue;

	// 接続状態を保持するフラグ
	bool bIsDeviceConnected;
};