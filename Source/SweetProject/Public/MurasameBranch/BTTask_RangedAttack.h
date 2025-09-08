// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RangedAttack.generated.h"

class AEnemyProjectile;

/**
 * 
 */
UCLASS()
class SWEETPROJECT_API UBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UBTTask_RangedAttack();

    /** 発射する弾 */
    UPROPERTY(EditAnywhere, Category = "Ranged")
    TSubclassOf<AEnemyProjectile> ProjectileClass;

    /** 発射する位置（Socket） */
    UPROPERTY(EditAnywhere, Category = "Ranged")
    FName MuzzleSocketName = TEXT("Muzzle");

    /** Socketない場合 */
    UPROPERTY(EditAnywhere, Category = "Ranged")
    FVector FallbackOffset = FVector(100.f, 0.f, 80.f);

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
