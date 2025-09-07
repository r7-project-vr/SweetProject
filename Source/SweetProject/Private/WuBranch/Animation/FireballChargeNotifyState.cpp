// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Animation/FireballChargeNotifyState.h"
#include "MurasameBranch/WitchBossActor.h"
#include "WuBranch/Actor/Component/SkillFireballComponent.h"

void UFireballChargeNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AWitchBossActor* Witch = Cast<AWitchBossActor>(MeshComp->GetOwner()))
		{
			Witch->UseFireball();
			FireBallComp = Cast<USkillFireballComponent>(Witch->GetComponentByClass(USkillFireballComponent::StaticClass()));
			if (FireBallComp)
			{
				FireBallComp->StartLockFireBall();
			}
		}
	}
}

void UFireballChargeNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp)
	{
		if (FireBallComp)
			FireBallComp->UnLockFireBall();
	}
}
