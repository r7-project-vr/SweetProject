#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../ASerial/ASerial_lib_Controller_Win.h"
#include "DeviceConnectorActor.generated.h"

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
    TUniquePtr<ASerial_lib_Controller_Win> ASerialController;
    TUniquePtr<WindowsSerial> SerialInterface;
    bool bIsDeviceConnected = false;

    void ParseSensorData(const uint8_t* data, uint8_t data_num);
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Device Data")
    FString ConnectionStatus;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Device Data")
    FQuat DeviceRotation;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Device Data")
    FVector DeviceAcceleration;
};