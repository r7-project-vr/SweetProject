// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateTargetAndDis.generated.h"

/**
 * 
 */
UCLASS()
class SWEETPROJECT_API UBTService_UpdateTargetAndDis : public UBTService
{
	GENERATED_BODY()
public:
    UBTService_UpdateTargetAndDis();
protected:
    UPROPERTY(EditAnywhere, Category = "Blackboard") FName KeyTarget = TEXT("TargetActor");
    UPROPERTY(EditAnywhere, Category = "Blackboard") FName KeyDistance = TEXT("DistanceToTarget");
	// 距離とクールタイムを　CanAttackに反映
    UPROPERTY(EditAnywhere, Category = "Blackboard") FName KeyCanAttack = TEXT("CanAttack");
    UPROPERTY(EditAnywhere, Category = "Blackboard") FName KeyLastAtk = TEXT("LastAttackTime");

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
