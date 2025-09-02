// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Controller/Behavior/BTService_UpdateState.h"
#include "AIController.h"
#include "MurasameBranch/WitchBossActor.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateState::UBTService_UpdateState()
{
	NodeName = "Update State";
}

void UBTService_UpdateState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
		return;

	ACharacter* MyCharacter = Controller->GetCharacter();
	if (!MyCharacter)
		return;

	AWitchBossActor* Witch = Cast<AWitchBossActor>(MyCharacter);
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (Witch)
	{
		Blackboard->SetValueAsBool(TEXT("IsStart"), Witch->GetIsStart());
	}
	else
	{
		Blackboard->SetValueAsBool(TEXT("IsStart"), false);
	}
}