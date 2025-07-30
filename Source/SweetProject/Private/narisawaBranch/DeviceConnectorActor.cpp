#include "narisawaBranch/DeviceConnectorActor.h"
#include "ASerial/ASerial_packet.h"
#include "HAL/UnrealMemory.h" 

ADeviceConnectorActor::ADeviceConnectorActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ADeviceConnectorActor::BeginPlay()
{
    Super::BeginPlay();
    ConnectToDevice();
}

void ADeviceConnectorActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    DisconnectFromDevice();
    Super::EndPlay(EndPlayReason);
}

void ADeviceConnectorActor::ConnectToDevice()
{
    // クラス名をヘッダファイル名に合わせてインスタンス化
    ASerialController = MakeUnique<ASerial_lib_Controller_Win>(0x01, 0x01);
    SerialInterface = MakeUnique<WindowsSerial>();
    ASerialController->SetInterfacePt(SerialInterface.Get());

    int connectedPort = ASerialController->AutoConnectDevice();

    if (connectedPort != -1)
    {
        ConnectionStatus = FString::Printf(TEXT("Successfully connected to COM%d"), connectedPort);
    }
    else
    {
        ConnectionStatus = TEXT("Failed to connect. Is the device connected?");
    }
}

void ADeviceConnectorActor::DisconnectFromDevice()
{
    if (ASerialController.IsValid())
    {
        ASerialController->DisConnectDevice();
        ConnectionStatus = TEXT("Disconnected");
    }
}

void ADeviceConnectorActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (ASerialController.IsValid() && ASerialController->GetConnectionState())
    {
        ReadAndProcessData();
    }
}

void ADeviceConnectorActor::ReadAndProcessData()
{
    static ASerialDataStruct::ASerialData receivedData;
    int status = ASerialController->ReadDataProcess(&receivedData);

    if (status == 1) // パケット受信成功
    {
        // デバイス側が送信するデータ形式に合わせて解析処理を実装
        // 例: コマンドID 0x20で、クォータニオン(16byte) + ボタン(1byte)が送られてくる場合
        if (receivedData.command == 0x20 && receivedData.data_num >= 17)
        {
            FMemory::Memcpy(&DeviceRotation, receivedData.data, sizeof(float) * 4);
            bIsActionButtonPressed = (receivedData.data[16] == 1);
        }
    }
}