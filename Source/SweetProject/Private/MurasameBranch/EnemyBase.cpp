// Fill out your copyright notice in the Description page of Project Settings.


#include "MurasameBranch/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AEnemyBase::AEnemyBase()
{
    //キャラクター移動引数
    PrimaryActorTick.bCanEverTick = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;

    // 2025.09.06 ウー start
    IsAttack = false;
    // 2025.09.06 ウー end
}

void AEnemyBase::BeginPlay()
{
    Super::BeginPlay();
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

float AEnemyBase::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    if (bIsDead) return 0.f;
    CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.f, Stats ? Stats->MaxHealth : 100.f);
    if (CurrentHealth <= 0.f)
    {
        bIsDead = true;
        GetCharacterMovement()->DisableMovement();
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
