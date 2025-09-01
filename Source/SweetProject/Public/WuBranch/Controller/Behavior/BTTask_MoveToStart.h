// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveToStart.generated.h"

/**
 * 
 */
UCLASS()
class SWEETPROJECT_API UBTTask_MoveToStart : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTTask_MoveToStart();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	/// <summary>
	/// 移動完了
	/// </summary>
	UFUNCTION()
	void OnMovementCompleted();
	
	UBehaviorTreeComponent* MyOwnerComp;

	/// <summary>
	/// 目標位置との許容範囲
	/// </summary>
	UPROPERTY(EditAnywhere)
	float AcceptanceRadius;
};
