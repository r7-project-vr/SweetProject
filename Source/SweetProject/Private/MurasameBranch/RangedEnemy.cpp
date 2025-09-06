// Fill out your copyright notice in the Description page of Project Settings.


#include "MurasameBranch/RangedEnemy.h"
#include "MurasameBranch/EnemyAIController.h"
#include "AIController.h"


ARangedEnemy::ARangedEnemy()
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = AEnemyAIController::StaticClass();
}

float ARangedEnemy::GetDesiredAttackRange_Implementation() const
{
    return Stats ? Stats->RangedRange : 1000.f;
}

/*
//DoRangedAttackの例
#include "EnemyProjectile.h"
#include "Kismet/GameplayStatics.h"

void ARangedEnemy::DoRangedAttack(AActor* Target)
{
    if (!Target) return;

    const FVector SpawnLoc = GetActorLocation() + GetActorForwardVector()*100.f + FVector(0,0,80.f);
    const FRotator SpawnRot = (Target->GetActorLocation() - SpawnLoc).Rotation();

    FActorSpawnParameters Params; Params.Owner = this; Params.Instigator = this;
    if (AEnemyProjectile* P = GetWorld()->SpawnActor<AEnemyProjectile>(AEnemyProjectile::StaticClass(), SpawnLoc, SpawnRot, Params))
    {
        // Hit用 DataAsset
        // P->Damage = GetDamage();
    }
}
*/