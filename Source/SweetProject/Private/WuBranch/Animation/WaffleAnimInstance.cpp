// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Animation/WaffleAnimInstance.h"
#include "MurasameBranch/RangedEnemy.h"

UWaffleAnimInstance::UWaffleAnimInstance()
	: Waffle(nullptr)
	, IsAlive(true)
	, IsAttack(false)
	, IsMove(false)
{
}

void UWaffleAnimInstance::UpdateAnimation(float DeltaTime)
{
	if (!Waffle)
		SetWaffleInstance();

	if (Waffle)
	{
		IsAlive = Waffle->IsAlive();
		IsAttack = Waffle->GetIsAttack();
		IsMove = Waffle->GetCurrentSpeed() > 0;
	}
}

void UWaffleAnimInstance::SetWaffleInstance()
{
	APawn* Target = TryGetPawnOwner();
	if (!Target)
	{
		return; 
	}

	if (ARangedEnemy* Actor = Cast<ARangedEnemy>(Target))
	{
		Waffle = Actor;
	}
}