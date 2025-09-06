// Fill out your copyright notice in the Description page of Project Settings.


#include "MurasameBranch/BTTask_MeleeAttack.h"
#include "MurasameBranch/EnemyBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    auto* C = OwnerComp.GetAIOwner();
    auto* E = C ? Cast<AEnemyBase>(C->GetPawn()) : nullptr;
    auto* BB = OwnerComp.GetBlackboardComponent();
    AActor* Target = BB ? Cast<AActor>(BB->GetValueAsObject(TEXT("TargetActor"))) : nullptr;

    if (!E || !Target || !E->IsAlive()) return EBTNodeResult::Failed;

    // 目標に向かって
    const FVector Dir = (Target->GetActorLocation() - E->GetActorLocation()).GetSafeNormal2D();
    E->SetActorRotation(Dir.Rotation());

    E->DoMeleeAttack(Target);

    // クールタイム記録用
    BB->SetValueAsFloat(TEXT("LastAttackTime"), E->GetWorld()->GetTimeSeconds());

    return EBTNodeResult::Succeeded;
}