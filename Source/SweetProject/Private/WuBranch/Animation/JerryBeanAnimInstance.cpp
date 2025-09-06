// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Animation/JerryBeanAnimInstance.h"
#include "MurasameBranch/MeleeEnemy.h"

UJerryBeanAnimInstance::UJerryBeanAnimInstance()
	: JerryBean(nullptr)
	, IsAlive(true)
	, IsAttack(false)
	, IsMove(false)
{
}

void UJerryBeanAnimInstance::UpdateAnimation(float DeltaTime)
{
	if (!JerryBean)
		SetJerryBeanInstance();

	if (JerryBean)
	{
		IsAlive = JerryBean->IsAlive();
		IsAttack = JerryBean->GetIsAttack();
		IsMove = JerryBean->GetCurrentSpeed() > 0;
	}
}

void UJerryBeanAnimInstance::SetJerryBeanInstance()
{
	if (AMeleeEnemy* Actor = Cast<AMeleeEnemy>(TryGetPawnOwner()))
	{
		JerryBean = Actor;
	}
}
