// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Animation/AttackCompleteNotify.h"
#include "MurasameBranch/EnemyBase.h"

void UAttackCompleteNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp || !MeshComp->GetOwner())
		return;

	if (AEnemyBase* Enemy = Cast<AEnemyBase>(MeshComp->GetOwner()))
	{
		Enemy->CompleteAttack();
	}
}
