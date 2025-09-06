// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MurasameBranch/EnemyBase.h"
#include "RangedEnemy.generated.h"

/**
 * 遠距離敵
 */
UCLASS()
class SWEETPROJECT_API ARangedEnemy : public AEnemyBase
{
	GENERATED_BODY()
public:
	ARangedEnemy();

	virtual float GetDesiredAttackRange_Implementation() const override;

	//Taskで弾発射するので　ここでDoRangedAttackをオーバーロードする必要がない
	//もしC＋＋で発射したいならここでDoRangedAttackをオーバーロードする
	//virtual void DoRangedAttack(AActor* Target) override;
	
};
