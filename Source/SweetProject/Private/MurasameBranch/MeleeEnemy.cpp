// Fill out your copyright notice in the Description page of Project Settings.


#include "MurasameBranch/MeleeEnemy.h"
#include "MurasameBranch/EnemyAIController.h"
#include "AIController.h"
// 2025.09.06 ウー start
#include "Components/BoxComponent.h"
#include "TokumaruBranch/Pawn/CPP_TVRPawn.h"
#include "Engine/DamageEvents.h"
// 2025.09.06 ウー end
#include "BehaviorTree/BlackboardComponent.h"
#include "MurasameBranch/EnemyProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AMeleeEnemy::AMeleeEnemy()
{
	//敵をAI持たせ
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//指定AIController
	AIControllerClass = AEnemyAIController::StaticClass();

	AttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack Collision"));
	AttackCollision->SetupAttachment(RootComponent);

	//Jump用
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->JumpZVelocity = JumpZVelocity;
	}
}

float AMeleeEnemy::GetDesiredAttackRange_Implementation() const
{
	//近接攻撃範囲
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

void AMeleeEnemy::Attack()
{
	// 2025.09.08 ウー start
	//AAIController* C = GetController<AAIController>();
	//AEnemyBase* E = C ? Cast<AEnemyBase>(C->GetPawn()) : nullptr;
	//UBlackboardComponent* BB = C->GetBlackboardComponent();
	//AActor* Target = BB ? Cast<AActor>(BB->GetValueAsObject(TEXT("TargetActor"))) : nullptr;

	//if (!E || !Target || !E->IsAlive()) return;

	//// 目標に向かって
	//const FVector Dir = (Target->GetActorLocation() - E->GetActorLocation()).GetSafeNormal2D();
	//E->SetActorRotation(Dir.Rotation());

	//E->DoMeleeAttack(Target);

	//// クールタイム記録用
	////BB->SetValueAsFloat(TEXT("LastAttackTime"), E->GetWorld()->GetTimeSeconds());

	//return;
	AEnemyAIController* EnemyController = GetController<AEnemyAIController>();
	EnemyController->TickMeleeJump();
	// 2025.09.08 ウー end
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
		OtherActor->TakeDamage(GetDamage(), DamageEvent, GetController(), this);
	}
}

//2025.09.16 得丸陽生
void AMeleeEnemy::SetHighlighted(bool enable, int number)
{
	if (USkeletalMeshComponent* lightMesh = GetMesh())
	{
		lightMesh->SetRenderCustomDepth(enable);
		if (enable)
		{
			lightMesh->CustomDepthStencilValue = 1;
			lightMesh->MarkRenderStateDirty();
		}
	}
}
//2025.09.16 得丸陽生 end

