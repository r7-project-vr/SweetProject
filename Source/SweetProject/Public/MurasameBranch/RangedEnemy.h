// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MurasameBranch/EnemyBase.h"
#include "RangedEnemy.generated.h"


class AEnemyProjectile;

/**
 * 遠距離敵
 */
UCLASS()
class SWEETPROJECT_API ARangedEnemy : public AEnemyBase
{
	GENERATED_BODY()
public:
	ARangedEnemy();

	//発射する弾
	UPROPERTY(EditAnywhere, Category = "Ranged")
	TSubclassOf<AEnemyProjectile> ProjectileClass;

	/** 発射する位置（Socket） */
	UPROPERTY(EditAnywhere, Category = "Ranged")
	FName MuzzleSocketName = TEXT("Muzzle");

	/** Socketない場合 */
	UPROPERTY(EditAnywhere, Category = "Ranged")
	FVector FallbackOffset = FVector(100.f, 0.f, 80.f);


	virtual float GetDesiredAttackRange_Implementation() const override;

	virtual void Attack() override;

	//Taskで弾発射するので　ここでDoRangedAttackをオーバーロードする必要がない
	//もしC＋＋で発射したいならここでDoRangedAttackをオーバーロードする
	//virtual void DoRangedAttack(AActor* Target) override;
	
private:
	//2025.09.16 得丸陽生
	void SetHighlighted(bool enable, int number) override;
	//2025.09.16 得丸陽生 end
};
