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

	//ジャンプのZ軸スピード
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
	float JumpZVelocity = 600.f;

	//ジャンプ時間間隔
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
	float JumpCooldown = 0.85f;

	//ジャンプの距離（上限
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
	float JumpStepDistance = 300.f;

	//近接敵とぷれいやー　の最短距離
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
	float JumpAcceptRadius = 150.f;

	//前回のジャンプ時間
	UPROPERTY(VisibleInstanceOnly, Category = "Jump")
	float LastJumpTime = -10000.f;


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

	//2025.09.16 得丸陽生
	void SetHighlighted(bool enable,int number) override;
	//2025.09.16 得丸陽生 end
};
