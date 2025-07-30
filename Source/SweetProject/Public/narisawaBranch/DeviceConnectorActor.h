#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASerial/WindowsSerial.h"
#include "ASerial/ASerial_lib_Controller_Win.h"
#include "DeviceConnectorActor.generated.h"

class ASerial_lib_Controller_Win; 
class WindowsSerial;

UCLASS()
class SWEETPROJECT_API ADeviceConnectorActor : public AActor
{
    GENERATED_BODY()

public:
    ADeviceConnectorActor();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    // 通信処理を行うクラスのインスタンスを保持するポインタ
    TUniquePtr<WindowsSerial> SerialInterface;
    TUniquePtr<ASerial_lib_Controller_Win> ASerialController;

    // データ受信と処理を行う内部関数
    void ReadAndProcessData();

public:
    // ブループリントから接続状態を確認するための変数
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASerial|Status")
    FString ConnectionStatus;

    // --- ブループリントに公開するデータ用の変数 (例) ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASerial|Data")
    FQuat DeviceRotation;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASerial|Data")
    FVector DeviceAcceleration;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASerial|Data")
    bool bIsActionButtonPressed;

    // --- ブループリントから呼び出す関数 ---
    UFUNCTION(BlueprintCallable, Category = "ASerial|Connection")
    void ConnectToDevice();

    UFUNCTION(BlueprintCallable, Category = "ASerial|Connection")
    void DisconnectFromDevice();
};