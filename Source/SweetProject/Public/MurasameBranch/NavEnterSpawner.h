// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavEnterSpawner.generated.h"

//class UBoxComponent;
class APawn;

UCLASS(Blueprintable)
class SWEETPROJECT_API ANavEnterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANavEnterSpawner();

protected:
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	// UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;

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

	//GetAllActorsOfClassですべてのクラス位置を取得
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	TSubclassOf<APawn> PlayerClass;

	//BeginPlayで自動的に生成する
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Run")
	bool bAutoRunOnBeginPlay = true;


	// //新規内容
	// //時間間隔
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Loop")
	// float SpawnInterval = 1.0f;
	//
	// //一回目の発動と生成の間のDelay、0だったらすぐ生成
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Loop")
	// float FirstDelay = 0.0f;
	//
	// //もしプレイヤーがトリガーに入っていないときOVERLAPタイマー停止するか
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Spawn|Loop")
	// bool bStopIfNoPawnInside = true;
	//
	// //追跡プレイヤー「ヒトツ」だけ
	// TWeakObjectPtr<APawn> TrackedPawn;
	//
	// FTimerHandle SpawnTimerHandle;



	// UFUNCTION()
	// void OnBoxBeginOverlap(
	// 	UPrimitiveComponent* OverlappedComponent,
	// 	AActor* OtherActor,
	// 	UPrimitiveComponent* OtherComp,
	// 	int32 OtherBodyIndex,
	// 	bool bFromSweep,
	// 	const FHitResult& SweepResult);
	//
	// UFUNCTION()
	// void OnBoxEndOverlap(
	// 	UPrimitiveComponent* OverLappedComponent,
	// 	AActor* OtherActor,
	// 	UPrimitiveComponent* OtherComp,
	// 	int32 OtherBodyIndex);
	//
	// void StartSpawningLoop(APawn* Pawn);
	// void StopSpawningLoop();
	// void SpawnOnce();//タイマーCallBack、一度生成
	//
	//
	// FVector PickGroundPointNearPlayer(AActor* Player);
	//
	// FVector PickAirPointUnderWitch();//魔女を中心に下半円空中ランダムスポット


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//プレイヤーアクターを解析
	AActor* ResolvePlayerActor() const;

	FVector PickGroundPointNearPlayer(AActor* Player) const;
	FVector PickAirPointUnderWitch() const; //魔女を中心に下半円空中ランダムスポット

public:
	//一回だけ
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void GenerateOnce();

	//外部関数は前回生成結果使用するとき
	UPROPERTY(BlueprintReadOnly, Category = "Spawn|Last")
	FVector LastStartPoint;

	UPROPERTY(BlueprintReadOnly, Category = "Spawn|Last")
	FVector LastEndPoint;

	// 2025.08.30 ウー start

	/// <summary>
	/// 地面判定の半径を設定
	/// </summary>
	/// <param name="Radius"></param>
	void SetGroundRadius(float Radius);

	/// <summary>
	/// 二つの(魔女とプレレイヤーの)近くの位置をゲット
	/// </summary>
	/// <param name="ONearWitch">魔女の位置</param>
	/// <param name="ONearPlayer">プレレイヤーの位置</param>
	void GetStartAndEndLocation(FVector& ONearWitch, FVector& ONearPlayer);

	// 2025.08.30 ウー end
};
