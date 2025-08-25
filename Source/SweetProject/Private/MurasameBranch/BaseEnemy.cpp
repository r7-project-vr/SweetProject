// Fill out your copyright notice in the Description page of Project Settings.


#include "MurasameBranch/BaseEnemy.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MaxHealth = 100.0f;
	Health = MaxHealth;

}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseEnemy::HandleDestruction()
{
	Destroy();
}

float ABaseEnemy::TakeDamage(float DamageAmount, 
	FDamageEvent const& DamageEvent, 
	AController* EventInstigator, 
	AActor* DamageCauser)
{
	float DamageToApply = 
		Super::TakeDamage(DamageAmount, 
			DamageEvent, EventInstigator, DamageCauser);
	DamageToApply =
		FMath::Min(Health,
			DamageToApply);
	Health -= DamageToApply;

	if (Health <= 0)
	{
		HandleDestruction();
	}

	return DamageToApply;
}
