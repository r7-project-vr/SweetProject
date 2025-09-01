// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Controller/Behavior/BTTask_SkillFireBall.h"
#include "WuBranch/Controller/WitchController.h"
#include "MurasameBranch/NavEnterSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "WuBranch/Actor/Meteorite.h"
#include "WuBranch/Actor/Component/SkillFireballComponent.h"

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

	AWitchController* AIController = Cast<AWitchController>(Controller);
	if (!AIController)
		return EBTNodeResult::Failed;
	
	ACharacter* MyCharacter = Controller->GetCharacter();
	if (!MyCharacter)
		return EBTNodeResult::Failed;

	USkeletalMeshComponent* Mesh = MyCharacter->GetMesh();
	if(!Mesh || !Mesh->DoesSocketExist(FireballSocketName))
		return EBTNodeResult::Failed;

	FVector SocketLocation = Mesh->GetSocketLocation(FireballSocketName);

	// 暫定
	AActor* Spawner = UGameplayStatics::GetActorOfClass(GetWorld(), ANavEnterSpawner::StaticClass());
	if (!Spawner)
	{
		Spawner = GetWorld()->SpawnActor(ANavEnterSpawner::StaticClass());
	}

	FVector NearWitchLocation, NearPlayerLocation;
	if (ANavEnterSpawner* LocationSpawner = Cast<ANavEnterSpawner>(Spawner))
	{
		LocationSpawner->GetStartAndEndLocation(NearWitchLocation, NearPlayerLocation);
	}

	if (USkillFireballComponent* FireballComp = MyCharacter->FindComponentByClass<USkillFireballComponent>())
	{
		AMeteorite* FireBall = FireballComp->SpawnFireBall(SocketLocation);
		if (FireBall)
			FireBall->SetTargetPoint(NearPlayerLocation);
	}

	return EBTNodeResult::Succeeded;
}
