// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "WitchController.generated.h"

class AMeteorite;
class USkeletalMeshComponent;

/**
 * 
 */
UCLASS()
class SWEETPROJECT_API AWitchController : public AAIController
{
	GENERATED_BODY()

public:

	AWitchController();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// 目標位置にたどり着いたか
	/// </summary>
	/// <param name="TargetLocation">目標位置</param>
	/// <returns>true: はい, false: いいえ</returns>
	bool IsArriveLocation(FVector TargetLocation);

	/// <summary>
	/// 目標位置に移動
	/// </summary>
	/// <param name="TargetLocation">目標位置</param>
	void StartMovingToLocation(FVector TargetLocation, float AcceptanceRadius);

	/// <summary>
	/// 移動中ですか
	/// </summary>
	/// <returns>true: はい, false: いいえ</returns>
	bool GetIsMoving() const;

	/// <summary>
	/// 移動完了の通知
	/// </summary>
	/// <param name=""></param>
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoveCompletedDelegate);

	FMoveCompletedDelegate OnMoveCompletedNotify;

private:

	/// <summary>
	/// ブラックボードの初期化
	/// </summary>
	void InitBlackboard();

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="DeltaTime"></param>
	void HandleMoving(float DeltaTime);

	/// <summary>
	/// 移動完了を通知する
	/// </summary>
	void NotifyMovementCompleted();

	/// <summary>
	/// ビヘイビアツリー
	/// </summary>
	UPROPERTY(EditAnywhere)
	UBehaviorTree* AIBehavior;

	/// <summary>
	/// ブラックボード
	/// </summary>
	class UBlackboardComponent* Blackboard;

	/// <summary>
	/// 移動中
	/// </summary>
	bool IsMoving;

	/// <summary>
	/// 目標位置
	/// </summary>
	FVector CurrentMoveTarget;

	/// <summary>
	/// 目標位置との許容範囲
	/// </summary>
	float DisplacementTolerance;

	/// <summary>
	/// 移動スピード
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MovementSpeed;

};
