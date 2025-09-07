// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetNewPatrolLocation.generated.h"

/**
 * 
 */
UCLASS()
class SWEETPROJECT_API UBTTask_SetNewPatrolLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UBTTask_SetNewPatrolLocation();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	/// <summary>
	/// 最低移動距離
	/// </summary>
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float MinMoveDistance;
};
