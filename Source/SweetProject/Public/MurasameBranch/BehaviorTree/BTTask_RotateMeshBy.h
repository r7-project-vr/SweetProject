// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateMeshBy.generated.h"

/**
 * AIのスケルトンメッシュを回転処理する、アクター本体をそのまま
 * 
 */
UCLASS()
class SWEETPROJECT_API UBTTask_RotateMeshBy : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RotateMeshBy();

	UPROPERTY(EditAnywhere, Category = "Rotation")
	float RotationYaw;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
