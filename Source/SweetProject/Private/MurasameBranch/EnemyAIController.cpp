// Fill out your copyright notice in the Description page of Project Settings.



#include "MurasameBranch/EnemyAIController.h"
#include "MurasameBranch/EnemyBase.h"                          // 敵のデータアセット用　State取得、AI配置ため
#include "Perception/AISense_Sight.h"
#include "Kismet/GameplayStatics.h"
// 2025.09.07 ウー start
#include <NavigationSystem.h>
// 2025.09.07 ウー end

AEnemyAIController::AEnemyAIController()
{
    // Perceptionと視覚
    Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
    SightCfg = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightCfg"));

    // 視覚デフォルト値（OnPossessでDataAssetの値を再配置）
    SightCfg->SightRadius = 2000.f;
    SightCfg->LoseSightRadius = 2200.f;
    SightCfg->PeripheralVisionAngleDegrees = 80.f;
    SightCfg->DetectionByAffiliation.bDetectEnemies = true;
    SightCfg->DetectionByAffiliation.bDetectFriendlies = true;
    SightCfg->DetectionByAffiliation.bDetectNeutrals = true;

    Perception->ConfigureSense(*SightCfg);
    Perception->SetDominantSense(SightCfg->GetSenseImplementation());
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // 敵のデータ資産に基づいて視覚パラメータをオーバーライドする
    if (AEnemyBase* Enemy = Cast<AEnemyBase>(InPawn))
    {
        if (Enemy->Stats)
        {
            SightCfg->SightRadius = Enemy->Stats->SightRadius;
            SightCfg->LoseSightRadius = Enemy->Stats->LoseSightRadius;
            SightCfg->PeripheralVisionAngleDegrees = Enemy->Stats->SightFOV;
            Perception->RequestStimuliListenerUpdate();
        }
    }

    // Perceptionバインドのcallback
    if (Perception)
    {
        Perception->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdated);
    }

	// BTとBBの初期化
    if (BehaviorTreeAsset && BehaviorTreeAsset->BlackboardAsset)
    {
        UBlackboardComponent* BB = nullptr;
        const bool bOK = UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BB);
        if (bOK && BB)
        {
            BlackboardComp = BB;
            RunBehaviorTree(BehaviorTreeAsset);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[EnemyAIController] UseBlackboard 失敗（BTまたはBB無効）"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[EnemyAIController] BTまたはBBは未設定"));
    }
}

void AEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& /*UpdatedActors*/)
{
    APawn* MyPawn = GetPawn();
    if (!MyPawn || !Perception) return;

	// 見れる全部のアクターを取得
    TArray<AActor*> SeenActors;
    Perception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), SeenActors);

    AActor* Best = nullptr;
    float BestDistSq = TNumericLimits<float>::Max();

    for (AActor* A : SeenActors)
    {
        // 距離
        const float D2 = FVector::DistSquared(A->GetActorLocation(), MyPawn->GetActorLocation());
        if (D2 < BestDistSq)
        {
            BestDistSq = D2;
            Best = A;
        }
    }

    // BBに入力
    UBlackboardComponent* BB = GetBlackboardComponent();
    if (BlackboardComp) BB = BlackboardComp.Get();

    if (BB)
    {
        BB->SetValueAsObject(KeyTarget, Best);
        BB->SetValueAsFloat(KeyDistance, Best ? FMath::Sqrt(BestDistSq) : 99999.f);
    }

    // Focus設定
    if (Best)
    {
        SetFocus(Best);
    }
    else
    {
        ClearFocus(EAIFocusPriority::Gameplay);
    }
}

// 2025.09.07 ウー start
FVector AEnemyAIController::GetNewPatrolLocation()
{
    // Navのデータをゲット
    FNavAgentProperties NavAgentProps;
    ANavigationData* NavData = UNavigationSystemV1::GetCurrent(GetWorld())->GetNavDataForProps(NavAgentProps);

    // 
    if (NavData)
    {
        FNavLocation ResultLocation;
        UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
        if (NavSystem && NavSystem->GetRandomReachablePointInRadius(GetPawn()->GetActorLocation(), 1000.f, ResultLocation, NavData))
        {
            return ResultLocation;
        }
    }
    return GetPawn()->GetActorLocation();
}
// 2025.09.07 ウー end
