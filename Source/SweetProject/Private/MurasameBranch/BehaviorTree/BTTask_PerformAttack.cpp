// Fill out your copyright notice in the Description page of Project Settings.


#include "MurasameBranch/BehaviorTree/BTTask_PerformAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MurasameBranch/EnemyBase.h"

UBTTask_PerformAttack::UBTTask_PerformAttack()
{
	NodeName = TEXT("Perform Attack (汎用)");
}

EBTNodeResult::Type UBTTask_PerformAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	AEnemyBase* EnemyPawn = Cast<AEnemyBase>(AIController->GetPawn());
	if (!EnemyPawn || !EnemyPawn->IsAlive())
	{
		return EBTNodeResult::Failed;
	}


	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}


	EnemyPawn->Attack();


	BlackboardComp->SetValueAsFloat(TEXT("LastAttackTime"), GetWorld()->GetTimeSeconds());


	return EBTNodeResult::Succeeded;
}
