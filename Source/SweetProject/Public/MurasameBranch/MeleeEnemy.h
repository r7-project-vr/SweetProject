// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MurasameBranch/EnemyBase.h"
#include "MeleeEnemy.generated.h"

class UBoxComponent;

/**
 * 近接敵
 */
UCLASS()
class SWEETPROJECT_API AMeleeEnemy : public AEnemyBase
{
	GENERATED_BODY()
public:
	AMeleeEnemy();

	virtual float GetDesiredAttackRange_Implementation() const override;

	virtual void Attack() override;

	// 2025.09.06 ウー start
protected:

	virtual void BeginPlay() override;

public:

	/// <summary>
	/// 攻撃の判定を起動
	/// </summary>
	void OpenAttackCollision();

	/// <summary>
	/// 攻撃の判定を終了
	/// </summary>
	void CloseAttackCollision();

private:

	/// <summary>
	/// 攻撃範囲に入ったら
	/// </summary>
	UFUNCTION()
	void OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// <summary>
	/// 攻撃範囲
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* AttackCollision;
	// 2025.09.06 ウー end

};
