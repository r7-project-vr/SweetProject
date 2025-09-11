// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "wuBranch/Enum/DonutLightStates.h"
#include "DonutLight.generated.h"

class USphereComponent;
class UPointLightComponent;

UCLASS()
class SWEETPROJECT_API ADonutLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADonutLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	/// <summary>
	/// 燃やせるものが近つく時
	/// </summary>
	UFUNCTION()
	void OnFireOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// <summary>
	/// ライトの強度を更新する
	/// </summary>
	/// <param name="Time">継続時間</param>
	void UpdateLightIntensity(float Time);

	/// <summary>
	/// 光をつける
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Light")
	void TurnOnLight();

	/// <summary>
	/// 光を消す
	/// </summary>
	void TurnOffLight();

	/// <summary>
	/// ドーナツのメッシュ
	/// </summary>
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DonutMesh;

	/// <summary>
	/// ライトのメッシュ
	/// </summary>
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LightMesh;

	/// <summary>
	/// ドーナツのコリジョン
	/// </summary>
	UPROPERTY(EditAnywhere)
	USphereComponent* Collision;

	/// <summary>
	/// ドーナツのライト
	/// </summary>
	UPROPERTY(EditAnywhere)
	UPointLightComponent* Light;

	// ライトの最大強度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MaxIntensity = 50.f; 

	/// <summary>
	/// 状態
	/// </summary>
	EDonutLightStates CurrentState;

	/// <summary>
	/// 点灯時間(単位：秒), 0:無限
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float GlowDuration;

	/// <summary>
	/// 時間のカウンター
	/// </summary>
	float TimeCounter = 0.f;

};
