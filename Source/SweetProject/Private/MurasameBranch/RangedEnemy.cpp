// Fill out your copyright notice in the Description page of Project Settings.


#include "MurasameBranch/RangedEnemy.h"
#include "MurasameBranch/EnemyAIController.h"
#include "MurasameBranch/EnemyProjectile.h"
//#include "BehaviorTree/BlackboardComponent.h"
//#include "Components/SkeletalMeshComponent.h"
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

void ARangedEnemy::Attack()
{
    AAIController* AI = GetController<AAIController>();

    if (!AI) return;


    // AEnemyBase* Enemy = Cast<AEnemyBase>(AI->GetPawn());
    // if (!Enemy || !Enemy->IsAlive()) return EBTNodeResult::Failed;
	if (!IsAlive()) return;

    UBlackboardComponent* BB = AI->GetBlackboardComponent();
    // if (!BB) return EBTNodeResult::Failed;
	if (!BB) return;

    AActor* Target = Cast<AActor>(BB->GetValueAsObject(TEXT("TargetActor")));
    if (!Target || !ProjectileClass) return;/* EBTNodeResult::Failed;*/

    // 発射する位置と向きの決定
    FVector SpawnLoc;
    FRotator SpawnRot;

    if (USkeletalMeshComponent* SelfMesh = GetMesh())
    {
        if (MuzzleSocketName != NAME_None && SelfMesh->DoesSocketExist(MuzzleSocketName))
        {
            const FTransform MuzzleTF = SelfMesh->GetSocketTransform(MuzzleSocketName, RTS_World);
            SpawnLoc = MuzzleTF.GetLocation();
            SpawnRot = (Target->GetActorLocation() - SpawnLoc).Rotation();
        }
        else
        {
            SpawnLoc = /*Enemy->*/GetActorLocation() + /*Enemy->*/GetActorForwardVector() * FallbackOffset.X
                + /*Enemy->*/GetActorRightVector() * FallbackOffset.Y
                + /*Enemy->*/GetActorUpVector() * FallbackOffset.Z;
            SpawnRot = (Target->GetActorLocation() - SpawnLoc).Rotation();
        }
    }
    else
    {
        SpawnLoc = /*Enemy->*/GetActorLocation() + /*Enemy->*/GetActorForwardVector() * FallbackOffset.X
            + /*Enemy->*/GetActorRightVector() * FallbackOffset.Y
            + /*Enemy->*/GetActorUpVector() * FallbackOffset.Z;
        SpawnRot = (Target->GetActorLocation() - SpawnLoc).Rotation();
    }

    // FActorSpawnParameters Params;
    // Params.Owner = Enemy;
    // Params.Instigator = Enemy; //　ダメージ受ける側を確定

    AEnemyProjectile* Proj = /*Enemy->*/GetWorld()->SpawnActor<AEnemyProjectile>(ProjectileClass, SpawnLoc, SpawnRot/*, Params*/);
    if (!Proj)
    {
        UE_LOG(LogTemp, Warning, TEXT("[BTTask_RangedAttack] Spawn Projectile failed."));
        return; //EBTNodeResult::Failed;
    }

    // ダメージ取得
    Proj->Damage = /*Enemy->*/GetDamage();

    // // クールタイム記録用
    // BB->SetValueAsFloat(TEXT("LastAttackTime"), Enemy->GetWorld()->GetTimeSeconds());

    //return EBTNodeResult::Succeeded;
}

//2025.09.16 得丸陽生
void ARangedEnemy::SetHighlighted(bool enable, int number)
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