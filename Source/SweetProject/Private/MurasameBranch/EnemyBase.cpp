// Fill out your copyright notice in the Description page of Project Settings.


#include "MurasameBranch/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
//9-10追加AIPerception
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
//9-10追加AIPerception

AEnemyBase::AEnemyBase()
{
    //キャラクター移動引数
    PrimaryActorTick.bCanEverTick = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;

    // 2025.09.06 ウー start
    IsAttack = false;
    // 2025.09.06 ウー end

    //9-10追加AIPerception
    Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
    SightCfg = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightCfg"));
    SightCfg->SightRadius = 2000.f;
    SightCfg->LoseSightRadius = 2200.f;
    SightCfg->PeripheralVisionAngleDegrees = 80.f;
    SightCfg->DetectionByAffiliation.bDetectEnemies = true;
    SightCfg->DetectionByAffiliation.bDetectFriendlies = true;
    SightCfg->DetectionByAffiliation.bDetectNeutrals = true;

    Perception->ConfigureSense(*SightCfg);
    Perception->SetDominantSense(SightCfg->GetSenseImplementation());


    //9-10追加AIPerception
}

void AEnemyBase::BeginPlay()
{
    Super::BeginPlay();

    //9-10追加AIPerception
    if (Perception) Perception->OnPerceptionUpdated.AddDynamic(this, &AEnemyBase::OnPerceptionUpdated);
    //9-10追加AIPerception

    if (Stats)
    {
        CurrentHealth = Stats->MaxHealth;
        GetCharacterMovement()->MaxWalkSpeed = Stats->MoveSpeed;
    }
    else
    {
        CurrentHealth = 100.f;
    }
}

//9-10追加AIPerception
void AEnemyBase::OnPerceptionUpdated(const TArray<AActor*>&)
{
	//avilable目標
    TArray<AActor*> Seen;
    Perception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), Seen);

    AActor* Best = nullptr; float BestD2 = TNumericLimits<float>::Max();
    for (AActor* A : Seen)
    {
        float D2 = FVector::DistSquared(A->GetActorLocation(), GetActorLocation());
        if (D2 < BestD2) { BestD2 = D2; Best = A; }

    }

    //PawnからControllerを取得
    if (AAIController* AI = Cast<AAIController>(GetController()))
    {
	    if (UBlackboardComponent* BB = AI->GetBlackboardComponent())
	    {
            BB->SetValueAsObject(TEXT("TargetActor"), Best);
            BB->SetValueAsVector(TEXT("TargetLocation"), Best ? Best->GetActorLocation() : FVector::ZeroVector);
            BB->SetValueAsFloat(TEXT("DistanceToTarget"), Best ? FMath::Sqrt(BestD2) : 99999.f);
	    }
    }


}
//9-10追加AIPerception


float AEnemyBase::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    // テスト
    if (bIsDead) return 0.f;
    CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.f, Stats ? Stats->MaxHealth : 100.f);
    if (CurrentHealth <= 0.f)
    {
        bIsDead = true;
        GetCharacterMovement()->DisableMovement();
        // 2025.09.09 ウー start
        NotifyDead();
        // 2025.09.09 ウー end
        //死亡エフェクトとか音とか追加ところ
        SetLifeSpan(5.f);
    }
    return DamageAmount;
}

void AEnemyBase::DoMeleeAttack(AActor* Target)
{
    if (!Target || bIsDead) return;

	// 近接攻撃の当たり判定
    FVector Start = GetActorLocation() + GetActorForwardVector() * 100.f;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(Stats ? Stats->MeleeRange : 150.f);

    TArray<FHitResult> Hits;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(Melee), false, this);

    bool bHit = GetWorld()->SweepMultiByChannel(
        Hits, Start, Start, FQuat::Identity, ECC_Pawn, Sphere, Params);

    if (bHit)
    {
        for (const FHitResult& Hr : Hits)
        {
            if (AActor* Victim = Hr.GetActor())
            {
                UGameplayStatics::ApplyDamage(Victim, GetDamage(), GetController(), this,
                    UDamageType::StaticClass());
            }
        }
    }
}

void AEnemyBase::DoRangedAttack(AActor* Target)
{
    // BTTask用
}

float AEnemyBase::GetDesiredAttackRange_Implementation() const
{
	//　近接攻撃距離を返す
    return Stats ? Stats->MeleeRange : 150.f;
}

float AEnemyBase::GetCurrentSpeed() const
{
    return GetCharacterMovement()->Velocity.Length();
}

// 2025.09.06 ウー start
void AEnemyBase::StartAttack()
{
    IsAttack = true;
}

void AEnemyBase::CompleteAttack()
{
    IsAttack = false;
}

bool AEnemyBase::GetIsAttack() const
{
    return IsAttack;
}
void AEnemyBase::NotifyDead()
{
    if (OnDeadEvent.IsBound())
        OnDeadEvent.Broadcast();
}
// 2025.09.06 ウー end
