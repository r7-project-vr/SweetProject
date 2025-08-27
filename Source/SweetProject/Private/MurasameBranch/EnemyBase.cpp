// Fill out your copyright notice in the Description page of Project Settings.


#include "MurasameBranch/EnemyBase.h"
#include "Kismet/GameplayStatics.h"

AEnemyBase::AEnemyBase() { PrimaryActorTick.bCanEverTick = false; }
void AEnemyBase::BeginPlay() { Super::BeginPlay(); }

bool AEnemyBase::CanAttack() const
{
	const float Now = UGameplayStatics::GetTimeSeconds(GetWorld());
	return (Now - LastAttackTime) >= AttackInterval;
}

void AEnemyBase::MarkJustAttacked()
{
	LastAttackTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	
}


