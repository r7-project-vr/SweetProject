// Fill out your copyright notice in the Description page of Project Settings.


#include "MurasameBranch/BehaviorTree/BTTask_RotateMeshBy.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

UBTTask_RotateMeshBy::UBTTask_RotateMeshBy()
{
	NodeName = TEXT("Rotate Mesh By");
	RotationYaw = 180.0f;
}

EBTNodeResult::Type UBTTask_RotateMeshBy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	
	ACharacter* MyCharacter = Controller->GetCharacter();
	if (!MyCharacter)
	{
		return EBTNodeResult::Failed;
	}

	// SkeletalonMeshゲット
	USkeletalMeshComponent* MeshComponent = MyCharacter->GetMesh();
	if (!MeshComponent)
	{
		return EBTNodeResult::Failed;
	}

	// 今のRotation取得
	const FRotator CurrentRelativeRotation = MeshComponent->GetRelativeRotation();

	// 新しいRotation決算
	FRotator NewRelativeRotation = CurrentRelativeRotation;
	NewRelativeRotation.Yaw += RotationYaw;

	// 新しいRotationアタッチ
	MeshComponent->SetRelativeRotation(NewRelativeRotation);

	return EBTNodeResult::Succeeded;
}
