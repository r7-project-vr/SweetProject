// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/Component/SkillFireballComponent.h"
#include "GameFramework/Character.h"
#include "WuBranch/Actor/Meteorite.h"

// Sets default values for this component's properties
USkillFireballComponent::USkillFireballComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	CurrentFireBall = nullptr;
}


// Called when the game starts
void USkillFireballComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	Mesh = Cast<ACharacter>(GetOwner())->GetMesh();
}


// Called every frame
void USkillFireballComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	LockFireBall();
}

AMeteorite* USkillFireballComponent::SpawnFireBall(FVector Location, FRotator Rotator)
{
	if (!FireBallSample)
		return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	AMeteorite* FireBall = GetOwner()->GetWorld()->SpawnActor<AMeteorite>(FireBallSample, Location, Rotator, SpawnParams);
	if (FireBall)
	{
		CurrentFireBall = FireBall;
		CurrentFireBall->SetAttackRangeRadius(AttackRadius);
	}
	return FireBall;
}

void USkillFireballComponent::Shoot()
{
	if (CurrentFireBall)
	{
		CurrentFireBall->Shoot();
		CurrentFireBall = nullptr;
	}
}

float USkillFireballComponent::GetAttackRadius() const
{
	return AttackRadius;
}

void USkillFireballComponent::StartLockFireBall()
{
	IsLockedFireBallLocation = true;
}

void USkillFireballComponent::UnLockFireBall()
{
	IsLockedFireBallLocation = false;
}

void USkillFireballComponent::LockFireBall()
{
	if (IsLockedFireBallLocation && CurrentFireBall && Mesh)
	{
		if (Mesh->DoesSocketExist(FireballSocketName))
		{
			FVector Location = Mesh->GetSocketLocation(FireballSocketName);
			CurrentFireBall->SetActorLocation(Location);
		}
	}
}