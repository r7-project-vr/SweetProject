// Fill out your copyright notice in the Description page of Project Settings.


#include "MurasameBranch/BTService_UpdateTargetAndDis.h"
#include "MurasameBranch/EnemyBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

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
    AEnemyBase* Enemy = AI ? Cast<AEnemyBase>(AI->GetPawn()) : nullptr;
    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!AI || !Enemy || !BB) return;

	// 一番近い敵「プレイヤー」を探す
    AActor* Best = Cast<AActor>(BB->GetValueAsObject(KeyTarget));
    if (!Best)
    {
        if (UAIPerceptionComponent* Per = AI->FindComponentByClass<UAIPerceptionComponent>())
        {
            TArray<AActor*> Seen; Per->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), Seen);
            float BestD2 = TNumericLimits<float>::Max();
            for (AActor* A : Seen)
            {
                const float D2 = FVector::DistSquared(A->GetActorLocation(), Enemy->GetActorLocation());
                if (D2 < BestD2) { BestD2 = D2; Best = A; }
            }
            if (Best) BB->SetValueAsObject(KeyTarget, Best);
        }
    }

    // Distance設定
    const float Dist = Best ? FVector::Dist(Best->GetActorLocation(), Enemy->GetActorLocation()) : 99999.f;
    BB->SetValueAsFloat(KeyDistance, Dist);

    // CanAttack設定
    if (KeyCanAttack != NAME_None)
    {
        const float Now = Enemy->GetWorld()->GetTimeSeconds();
        const float Last = BB->GetValueAsFloat(KeyLastAtk);
        const bool NotInCD = (Now - Last) >= Enemy->GetAttackInterval();

        const float Range = Enemy->GetDesiredAttackRange();
		const bool InRange = Dist <= Range * 1.05f;// 少し余裕を持たせる

        BB->SetValueAsBool(KeyCanAttack, (NotInCD && InRange));
    }
}

