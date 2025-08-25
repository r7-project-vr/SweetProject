#include "narisawaBranch/DeviceConnectorActor.h"
#include "../ASerial/WindowsSerial.h"

ADeviceConnectorActor::ADeviceConnectorActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ADeviceConnectorActor::BeginPlay()
{
    Super::BeginPlay();
    ASerialController = MakeUnique<ASerial_lib_Controller_Win>(0x10, 0x01); // ID: 0x10
    SerialInterface = MakeUnique<WindowsSerial>();
    ASerialController->SetInterfacePt(SerialInterface.Get());

    const int32 ConnectedComPort = ASerialController->AutoConnectDevice();
    if (ConnectedComPort != -1)
    {
        bIsDeviceConnected = true;
        ConnectionStatus = FString::Printf(TEXT("OK: Connected on COM%d"), ConnectedComPort);
        ASerialController->WriteData(0x20);
    }
    else
    {
        bIsDeviceConnected = false;
        ConnectionStatus = TEXT("ERROR: Could not find device.");
    }
}

void ADeviceConnectorActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bIsDeviceConnected && ASerialController.IsValid())
    {
        static ASerialDataStruct::ASerialData receivedData;
        const int32 ReadStatus = ASerialController->ReadDataProcess(&receivedData);

        if (ReadStatus == 1 && receivedData.command == 0x20)
        {
            ParseSensorData(receivedData.data, receivedData.data_num);
        }
    }
}

void ADeviceConnectorActor::ParseSensorData(const uint8_t* data, uint8_t data_num)
{
    // 回転16バイト + 加速度12バイト = 28バイト以上あるかチェック
    if (data_num < 28) return;

    // FQuatとFVectorはUEの標準型なので、そのまま使える
    FQuat rotation;
    FVector acceleration;

    // メモリコピーでバイト配列を構造体に変換
    FMemory::Memcpy(&rotation, &data[0], sizeof(FQuat));
    FMemory::Memcpy(&acceleration, &data[16], sizeof(FVector));

    // UPROPERTYの変数に格納
    DeviceRotation = rotation;
    DeviceAcceleration = acceleration;
}

void ADeviceConnectorActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (ASerialController.IsValid() && bIsDeviceConnected)
    {
        ASerialController->DisConnectDevice();
    }
    Super::EndPlay(EndPlayReason);
}