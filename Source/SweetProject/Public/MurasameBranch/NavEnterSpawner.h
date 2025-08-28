// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavEnterSpawner.generated.h"

class UBoxComponent;
class APawn;

UCLASS(Blueprintable)
class SWEETPROJECT_API ANavEnterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANavEnterSpawner();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* Box;

	//魔女のアクター指定
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TWeakObjectPtr<AActor> WitchActor;

	//地面判定終点
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float GroundRadius = 100.f;

	//空中判定始点
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float AirRadius = 50.f;

	//地面でランダムスポット探す
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	bool bUseNavQuery = true;


	//新規内容
	//時間間隔
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Loop")
	float SpawnInterval = 1.0f;

	//一回目の発動と生成の間のDelay、0だったらすぐ生成
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Loop")
	float FirstDelay = 0.0f;

	//もしプレイヤーがトリガーに入っていないときOVERLAPタイマー停止するか
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Spawn|Loop")
	bool bStopIfNoPawnInside = true;

	//追跡プレイヤー「ヒトツ」だけ
	TWeakObjectPtr<APawn> TrackedPawn;

	FTimerHandle SpawnTimerHandle;



	UFUNCTION()
	void OnBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(
		UPrimitiveComponent* OverLappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	void StartSpawningLoop(APawn* Pawn);
	void StopSpawningLoop();
	void SpawnOnce();//タイマーCallBack、一度生成


	FVector PickGroundPointNearPlayer(AActor* Player);

	FVector PickAirPointUnderWitch();//魔女を中心に下半円空中ランダムスポット


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};
