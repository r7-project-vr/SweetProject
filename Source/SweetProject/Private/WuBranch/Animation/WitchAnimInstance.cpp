// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Animation/WitchAnimInstance.h"
#include "MurasameBranch/WitchBossActor.h"

UWitchAnimInstance::UWitchAnimInstance()
	: Witch(nullptr)
	, IsAttack(false)
{
}

void UWitchAnimInstance::UpdateAnimation(float DeltaTime)
{
	if (!Witch)
		SetWitchInstance();

	if(Witch)
		IsAttack = Witch->GetIsAttack();
}

void UWitchAnimInstance::SetWitchInstance()
{
	if (AWitchBossActor* Actor = Cast<AWitchBossActor>(TryGetPawnOwner()))
	{
		Witch = Actor;
	}
}
