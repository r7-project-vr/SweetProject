// このコードでファイル全体を完全に上書きしてください
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ASerialLibControllerWin.h"

// ★★★ 修正: .generated.h のインクルードを一番下に移動 ★★★
#include "ASerialComBPLibrary.generated.h" 

// 前方宣言: これからこういうクラスを使いますよ、とコンパイラに教える
class ASerial_lib_Controller_Win;
class WindowsSerial;

// --- データ受信イベントの型を宣言 ---
// 回転(FRotator), 加速度(FVector), ボタン(bool)の3つのパラメータを持つイベントを定義
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSensorDataReceived, const FRotator&, Rotation, const FVector&, Acceleration, bool, bIsButtonPressed);

UCLASS()
class UASerialComBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// --- ブループリントでアクセスするためのイベント ---
	UPROPERTY(BlueprintAssignable, Category = "ASerialCom")
	static FOnSensorDataReceived OnSensorDataReceived;

	// --- ブループリントから呼び出す関数 ---

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Connect to ASerial Device", Keywords = "ASerial connect"), Category = "ASerialCom")
	static int32 Connect(int32 DeviceId = 10, int32 DeviceVersion = 1);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Disconnect ASerial Device", Keywords = "ASerial disconnect"), Category = "ASerialCom")
	static void Disconnect();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Request Sensor Data", Keywords = "ASerial request data"), Category = "ASerialCom")
	static void RequestSensorData();

private:
	// --- 内部処理用の変数 ---
	static TUniquePtr<ASerial_lib_Controller_Win> ASerialController;
	static TUniquePtr<WindowsSerial> SerialInterface;
	static bool bIsConnected;
	static FTickerDelegate TickDelegate;
	static FDelegateHandle TickHandle;

	// 毎フレーム、デバイスからのデータをチェックする関数
	static bool Tick(float DeltaTime);

	// 受信したバイトデータを解析する関数
	static void ParseSensorData(const uint8* data, uint8 data_num);
};