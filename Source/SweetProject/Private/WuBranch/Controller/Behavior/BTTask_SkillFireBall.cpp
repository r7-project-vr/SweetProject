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

	if (!OwnerComp.GetAIOwner())
	{
		return EBTNodeResult::Failed;
	}

	AWitchBossActor* Witch = Cast<AWitchBossActor>(OwnerComp.GetAIOwner()->GetPawn());
	
	return EBTNodeResult::Type();
}

