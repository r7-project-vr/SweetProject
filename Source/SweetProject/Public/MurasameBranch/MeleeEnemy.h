// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MurasameBranch/EnemyBase.h"
#include "MeleeEnemy.generated.h"

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
};
