// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SkillFireBall.generated.h"

/**
 * 
 */
UCLASS()
class SWEETPROJECT_API UBTTask_SkillFireBall : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UBTTask_SkillFireBall();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
