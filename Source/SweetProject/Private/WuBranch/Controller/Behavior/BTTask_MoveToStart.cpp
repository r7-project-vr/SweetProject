// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Controller/Behavior/BTTask_MoveToStart.h"
#include "WuBranch/Controller/WitchController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveToStart::UBTTask_MoveToStart()
	: MyOwnerComp(nullptr)
{
	NodeName = "Move To Start Location";

	// BlackboardKeyをVector型に限定
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_MoveToStart, BlackboardKey));
}

EBTNodeResult::Type UBTTask_MoveToStart::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
		return EBTNodeResult::Failed;

	AWitchController* AIController = Cast<AWitchController>(Controller);
	if(!AIController)
		return EBTNodeResult::Failed;

	// 既に移動中の場合
	if (AIController->GetIsMoving())
		return EBTNodeResult::InProgress;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if(!Blackboard || !GetSelectedBlackboardKey().IsValid())
		return EBTNodeResult::Failed;

	MyOwnerComp = &OwnerComp;
	AIController->OnMoveCompletedNotify.AddDynamic(this, &UBTTask_MoveToStart::OnMovementCompleted);
	FVector TargetLocation = Blackboard->GetValueAsVector(GetSelectedBlackboardKey());
	AIController->StartMovingToLocation(TargetLocation, AcceptanceRadius);

	return EBTNodeResult::InProgress;
}

void UBTTask_MoveToStart::OnMovementCompleted()
{
	// バインド解除
	AWitchController* AIController = Cast<AWitchController>(MyOwnerComp->GetAIOwner());
	AIController->OnMoveCompletedNotify.RemoveDynamic(this, &UBTTask_MoveToStart::OnMovementCompleted);

	UBlackboardComponent* Blackboard = MyOwnerComp->GetBlackboardComponent();
	Blackboard->SetValueAsBool(TEXT("IsArriveStartLocation"), true);

	FinishLatentTask(*MyOwnerComp, EBTNodeResult::Succeeded);
	MyOwnerComp = nullptr;
}

