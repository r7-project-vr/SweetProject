// Source/SweetProject/Public/narisawaBranch/DeviceActor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Queue.h" // TQueueの使用に必要
#include "DeviceActor.generated.h"

// 前方宣言
class UASerialLibControllerWin;
class FRunnableThread;
class WindowsSerial;

// スレッドとメインスレッド間でデータを安全に受け渡すための構造体
// ヘッダファイル内でTQueueの型として使用するため、UCLASSの前に定義します
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
	void ProcessReceivedData(const FSerialData& Data);

	UPROPERTY()
	TObjectPtr<UASerialLibControllerWin> SerialController;

	WindowsSerial* SerialInterface;
	class FDeviceCommunicationTask* CommunicationTask;
	FRunnableThread* CommunicationThread;
	TQueue<FSerialData, EQueueMode::Spsc> DataQueue;
	bool bIsDeviceConnected;
};