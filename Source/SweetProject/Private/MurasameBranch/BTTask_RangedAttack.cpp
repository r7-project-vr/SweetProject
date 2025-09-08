// Fill out your copyright notice in the Description page of Project Settings.


#include "MurasameBranch/BTTask_RangedAttack.h"
//#include "MurasameBranch/EnemyBase.h"
//#include "MurasameBranch/EnemyProjectile.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "Components/SkeletalMeshComponent.h"
//#include "Kismet/GameplayStatics.h"
#include "MurasameBranch/RangedEnemy.h"


UBTTask_RangedAttack::UBTTask_RangedAttack()
{
    NodeName = TEXT("Ranged Attack");
}

EBTNodeResult::Type UBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
		return EBTNodeResult::Failed;

	ACharacter* MyCharacter = Controller->GetCharacter();
	if (!MyCharacter)
		return EBTNodeResult::Failed;

	if (ARangedEnemy* RangedEnemy = Cast<ARangedEnemy>(MyCharacter))
		RangedEnemy->StartAttack();
	else
		return EBTNodeResult::Failed;
	
	return EBTNodeResult::Succeeded;


 //   AAIController* AI = OwnerComp.GetAIOwner();
 //   if (!AI) return EBTNodeResult::Failed;

 //   AEnemyBase* Enemy = Cast<AEnemyBase>(AI->GetPawn());
 //   if (!Enemy || !Enemy->IsAlive()) return EBTNodeResult::Failed;

 //   UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
 //   if (!BB) return EBTNodeResult::Failed;

 //   AActor* Target = Cast<AActor>(BB->GetValueAsObject(TEXT("TargetActor")));
 //   if (!Target || !ProjectileClass) return EBTNodeResult::Failed;

	//// 発射する位置と向きの決定
 //   FVector SpawnLoc;
 //   FRotator SpawnRot;

 //   if (USkeletalMeshComponent* Mesh = Enemy->GetMesh())
 //   {
 //       if (MuzzleSocketName != NAME_None && Mesh->DoesSocketExist(MuzzleSocketName))
 //       {
 //           const FTransform MuzzleTF = Mesh->GetSocketTransform(MuzzleSocketName, RTS_World);
 //           SpawnLoc = MuzzleTF.GetLocation();
 //           SpawnRot = (Target->GetActorLocation() - SpawnLoc).Rotation();
 //       }
 //       else
 //       {
 //           SpawnLoc = Enemy->GetActorLocation() + Enemy->GetActorForwardVector() * FallbackOffset.X
 //               + Enemy->GetActorRightVector() * FallbackOffset.Y
 //               + Enemy->GetActorUpVector() * FallbackOffset.Z;
 //           SpawnRot = (Target->GetActorLocation() - SpawnLoc).Rotation();
 //       }
 //   }
 //   else
 //   {
 //       SpawnLoc = Enemy->GetActorLocation() + Enemy->GetActorForwardVector() * FallbackOffset.X
 //           + Enemy->GetActorRightVector() * FallbackOffset.Y
 //           + Enemy->GetActorUpVector() * FallbackOffset.Z;
 //       SpawnRot = (Target->GetActorLocation() - SpawnLoc).Rotation();
 //   }

 //   FActorSpawnParameters Params;
 //   Params.Owner = Enemy;
 //   Params.Instigator = Enemy; //　ダメージ受ける側を確定

 //   AEnemyProjectile* Proj = Enemy->GetWorld()->SpawnActor<AEnemyProjectile>(ProjectileClass, SpawnLoc, SpawnRot, Params);
 //   if (!Proj)
 //   {
 //       UE_LOG(LogTemp, Warning, TEXT("[BTTask_RangedAttack] Spawn Projectile failed."));
 //       return EBTNodeResult::Failed;
 //   }

 //   // ダメージ取得
 //   Proj->Damage = Enemy->GetDamage();

	//// クールタイム記録用
 //   BB->SetValueAsFloat(TEXT("LastAttackTime"), Enemy->GetWorld()->GetTimeSeconds());

 //   return EBTNodeResult::Succeeded;
}