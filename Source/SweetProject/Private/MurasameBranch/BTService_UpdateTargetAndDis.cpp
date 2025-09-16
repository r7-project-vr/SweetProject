// Fill out your copyright notice in the Description page of Project Settings.


#include "MurasameBranch/BTService_UpdateTargetAndDis.h"
#include "MurasameBranch/EnemyBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include <TokumaruBranch/Pawn/CPP_TVRPawn.h>

UBTService_UpdateTargetAndDis::UBTService_UpdateTargetAndDis()
{
    NodeName = TEXT("Update Target & Distance");
	Interval = 0.2f; // 0.2秒ごとに実行
    bNotifyBecomeRelevant = false;
}


void UBTService_UpdateTargetAndDis::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	AAIController* AI = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AI || !BB) return;

	AEnemyBase* Enemy = Cast<AEnemyBase>(AI->GetPawn());
	if (!Enemy) return;


	ACPP_TVRPawn* Target = Cast<ACPP_TVRPawn>(BB->GetValueAsObject(KeyTarget));


	const float Dist = Target ? FVector::Dist(Target->GetActorLocation(), Enemy->GetActorLocation()) :
		TNumericLimits<float>::Max();
	BB->SetValueAsFloat(KeyDistance, Dist);

	//CanAttack設定
	if (KeyCanAttack != NAME_None)
	{
		if (!Target)
		{
			BB->SetValueAsBool(KeyCanAttack, false);
			return;
		}
	}

	const float Now = Enemy->GetWorld()->GetTimeSeconds();
	const float Last = BB->GetValueAsFloat(KeyLastAtk);
	const bool NotInCD = (Now - Last) >= Enemy->GetAttackInterval();

	const float Range = Enemy->GetDesiredAttackRange();
	const bool InRange = Dist <= Range * 1.05f;//少し余裕を持たせる

	BB->SetValueAsBool(KeyCanAttack, (NotInCD && InRange));


 //   Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

 //   AAIController* AI = OwnerComp.GetAIOwner();
 //   AEnemyBase* Enemy = AI ? Cast<AEnemyBase>(AI->GetPawn()) : nullptr;
 //   UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
 //   if (!AI || !Enemy || !BB) return;



	//// 一番近い敵「プレイヤー」を探す
 //   ACPP_TVRPawn* Best = Cast<ACPP_TVRPawn>(BB->GetValueAsObject(KeyTarget));
 //   if (!Best)
 //   {
 //       if (UAIPerceptionComponent* Per = AI->FindComponentByClass<UAIPerceptionComponent>())
 //       {
 //           TArray<AActor*> Seen; 
 //           Per->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), Seen);
 //           float BestD2 = TNumericLimits<float>::Max();
 //           for (AActor* A : Seen)
 //           {
 //               if (ACPP_TVRPawn* Player = Cast<ACPP_TVRPawn>(A))
 //               {
 //                   const float D2 = FVector::DistSquared(A->GetActorLocation(), Enemy->GetActorLocation());
 //                   if (D2 < BestD2) { BestD2 = D2; Best = Player; }
 //               }
 //           }
 //           
 //           if (Best) BB->SetValueAsObject(KeyTarget, Best);    
 //       }
 //   }

 //   
 //   // Distance設定
 //   const float Dist = Best ? FVector::Dist(Best->GetActorLocation(), Enemy->GetActorLocation()) : 99999.f;
 //   BB->SetValueAsFloat(KeyDistance, Dist);

 //   // CanAttack設定
 //   if (KeyCanAttack != NAME_None)
 //   {
 //       const float Now = Enemy->GetWorld()->GetTimeSeconds();
 //       const float Last = BB->GetValueAsFloat(KeyLastAtk);
 //       const bool NotInCD = (Now - Last) >= Enemy->GetAttackInterval();

 //       const float Range = Enemy->GetDesiredAttackRange();
	//	const bool InRange = Dist <= Range * 1.05f;// 少し余裕を持たせる

 //       BB->SetValueAsBool(KeyCanAttack, (NotInCD && InRange));
 //   }
}

