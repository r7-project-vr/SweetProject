// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Animation/JumpNotify.h"
#include <MurasameBranch/EnemyBase.h>
#include <MurasameBranch/EnemyAIController.h>

void UJumpNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp || !MeshComp->GetOwner())
		return;

	if (AEnemyBase* Enemy = Cast<AEnemyBase>(MeshComp->GetOwner()))
	{
		if (AEnemyAIController* Controller = Cast<AEnemyAIController>(Enemy->GetController()))
		{
			Controller->TickMeleeJump();
		}
	}
}
