// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Animation/DirectImpactNotifyState.h"
#include "MurasameBranch/MeleeEnemy.h"

void UDirectImpactNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AMeleeEnemy* Enemy = Cast<AMeleeEnemy>(MeshComp->GetOwner()))
		{
			Owner = Enemy;
			Enemy->OpenAttackCollision();
		}
	}
}

void UDirectImpactNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp)
	{
		if (Owner)
			Owner->CloseAttackCollision();
	}
}
