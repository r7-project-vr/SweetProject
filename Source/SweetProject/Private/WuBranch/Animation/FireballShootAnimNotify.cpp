// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Animation/FireballShootAnimNotify.h"
#include <MurasameBranch/WitchBossActor.h>

void UFireballShootAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp || !MeshComp->GetOwner())
		return;

	if (AWitchBossActor* Witch = Cast<AWitchBossActor>(MeshComp->GetOwner()))
	{
		Witch->Shoot();
	}
}
