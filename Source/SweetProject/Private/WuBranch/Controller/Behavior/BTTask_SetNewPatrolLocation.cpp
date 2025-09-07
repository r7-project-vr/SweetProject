// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Controller/Behavior/BTTask_SetNewPatrolLocation.h"
#include "MurasameBranch/EnemyAIController.h"

UBTTask_SetNewPatrolLocation::UBTTask_SetNewPatrolLocation()
{
	NodeName = "Set Patrol Location";

	// BlackboardKeyをVector型に限定
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SetNewPatrolLocation, BlackboardKey));
}

EBTNodeResult::Type UBTTask_SetNewPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
		return EBTNodeResult::Failed;

	AEnemyAIController* AIController = Cast<AEnemyAIController>(Controller);
	if (!AIController)
		return EBTNodeResult::Failed;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard || !GetSelectedBlackboardKey().IsValid())
		return EBTNodeResult::Failed;

	FVector Location = AIController->GetNewPatrolLocation(MinMoveDistance);
	Blackboard->SetValueAsVector(GetSelectedBlackboardKey(), Location);

	return EBTNodeResult::Type();
}
