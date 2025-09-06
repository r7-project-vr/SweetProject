// Fill out your copyright notice in the Description page of Project Settings.


#include "MurasameBranch/MeleeEnemy.h"
#include "MurasameBranch/EnemyAIController.h"
#include "AIController.h"
// 2025.09.06 ウー start
#include "Components/BoxComponent.h"
#include "TokumaruBranch/Pawn/CPP_TVRPawn.h"
#include "Engine/DamageEvents.h"
// 2025.09.06 ウー end

AMeleeEnemy::AMeleeEnemy()
{
	//敵をAI持たせ
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//指定AIController
	AIControllerClass = AEnemyAIController::StaticClass();

	AttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack Collision"));
	AttackCollision->SetupAttachment(RootComponent);
}

float AMeleeEnemy::GetDesiredAttackRange_Implementation() const
{
	//近接攻撃範囲です
	return Stats ? Stats->MeleeRange : 150.f;
}

void AMeleeEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (AttackCollision)
	{
		AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AMeleeEnemy::OnAttackOverlapBegin);
		CloseAttackCollision();
	}
		
}

void AMeleeEnemy::OpenAttackCollision()
{
	AttackCollision->Activate(true);
}

void AMeleeEnemy::CloseAttackCollision()
{
	AttackCollision->Activate(false);
}

void AMeleeEnemy::OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ACPP_TVRPawn::StaticClass()))
	{
		FDamageEvent DamageEvent(UDamageType::StaticClass());
		OtherActor->TakeDamage(100.f, DamageEvent, GetController(), this);
	}
}


