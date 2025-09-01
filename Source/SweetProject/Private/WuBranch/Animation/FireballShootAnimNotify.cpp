// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Animation/FireballShootAnimNotify.h"
#include <MurasameBranch/WitchBossActor.h>
#include <WuBranch/Actor/Component/SkillFireballComponent.h>

void UFireballShootAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp)
		return;

	if (!MeshComp->GetOwner())
		return;

	if (AWitchBossActor* Witch = Cast<AWitchBossActor>(MeshComp->GetOwner()))
	{
		if (USkillFireballComponent* FireBallComp = Cast<USkillFireballComponent>(Witch->GetComponentByClass(USkillFireballComponent::StaticClass())))
		{
			FireBallComp->Shoot();
		}
	}
}
