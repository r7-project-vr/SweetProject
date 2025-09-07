// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Controller/Behavior/BTTask_SkillFireBall.h"
#include "AIController.h"
#include "MurasameBranch/WitchBossActor.h"

UBTTask_SkillFireBall::UBTTask_SkillFireBall()
{
	NodeName = "Use Skill: Fire Ball";
}

EBTNodeResult::Type UBTTask_SkillFireBall::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
		return EBTNodeResult::Failed;
	
	ACharacter* MyCharacter = Controller->GetCharacter();
	if (!MyCharacter)
		return EBTNodeResult::Failed;

	if (AWitchBossActor* Witch = Cast<AWitchBossActor>(MyCharacter))
		Witch->StartAttack();
	else
		return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}
