// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DirectImpactNotifyState.generated.h"

class AMeleeEnemy;

/**
 * 
 */
UCLASS()
class SWEETPROJECT_API UDirectImpactNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:

	AMeleeEnemy* Owner;
};
