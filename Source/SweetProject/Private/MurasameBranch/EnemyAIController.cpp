// Fill out your copyright notice in the Description page of Project Settings.



#include "MurasameBranch/EnemyAIController.h"
#include "MurasameBranch/EnemyBase.h"                          // 敵のデータアセット用　State取得、AI配置ため
#include "MurasameBranch/MeleeEnemy.h"
#include "Perception/AISense_Sight.h"
#include "Kismet/GameplayStatics.h"
// 2025.09.07 ウー start
#include <NavigationSystem.h>
// 2025.09.07 ウー end
#include "NavigationPath.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <TokumaruBranch/Pawn/CPP_TVRPawn.h>

AEnemyAIController::AEnemyAIController()
{
    PrimaryActorTick.bCanEverTick = true;

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
            if(SightCfg)
            {
                SightCfg->SightRadius = Enemy->Stats->SightRadius;
                SightCfg->LoseSightRadius = Enemy->Stats->LoseSightRadius;
                SightCfg->PeripheralVisionAngleDegrees = Enemy->Stats->SightFOV;
                Perception->RequestStimuliListenerUpdate();
            }
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

void AEnemyAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    //TickMeleeJump();
}


void AEnemyAIController::TickMeleeJump()
{
    AMeleeEnemy* M = Cast<AMeleeEnemy>(GetPawn());
    if (!M) return;                                     //MeleeEnemyかを判断

    if (!BlackboardComp) BlackboardComp = GetBlackboardComponent();
    if (!BlackboardComp) return;

    FVector Aim = BlackboardComp->GetValueAsVector(KeyTarget);
    if (Aim.IsNearlyZero())
    {
        if (AActor* A = Cast<AActor>(BlackboardComp->GetValueAsObject(KeyTargetAct)))
        {
            Aim = A->GetActorLocation();
        }
    }
    if (Aim.IsNearlyZero()) return;


    const FVector From = M->GetActorLocation();
    const float Dist2D = FVector::Dist2D(From, Aim);
    const float Accept = M->JumpAcceptRadius > 0.f ? M->JumpAcceptRadius : (M->GetDesiredAttackRange() * 0.8f);
    if (Dist2D <= Accept) return;

    const float Now = GetWorld()->GetTimeSeconds();
    if ((Now - M->LastJumpTime) < M->JumpCooldown) return;

    if (UCharacterMovementComponent* Move = M->GetCharacterMovement())
    {
        if (Move->IsFalling()) return;
        if (!Move->IsMovingOnGround()) return;
    }


    FVector To = Aim;

    if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
    {
        if (UNavigationPath* Path = NavSys->FindPathToLocationSynchronously(GetWorld(), From, Aim, M))
        {
            if (Path && Path->PathPoints.Num() >= 2)
            {
                To = Path->PathPoints[1];
            }
        }
    }


    FVector LaunchVel;
    const float StepXY = FMath::Max(50.f, M->JumpStepDistance);
    const float JumpZ = M->JumpZVelocity > 0.f ? M->JumpZVelocity : 600.f;

    if (!ComputeJumpVelocity(From, To, JumpZ, StepXY, LaunchVel))
    {
        return; //目標が近すぎ
    }


    M->LaunchCharacter(LaunchVel, true, true);
    M->LastJumpTime = Now;
}

bool AEnemyAIController::ComputeJumpVelocity(const FVector& From, const FVector& To,
    float JumpZ, float StepXY, FVector& OutVel) const
{
    const FVector Dir2D = (To - From).GetSafeNormal2D();
    float Dist2D = FVector::Dist2D(From, To);


    Dist2D = FMath::Min(Dist2D, StepXY);
    if (Dist2D < 10.f) return false;

    const float G = FMath::Abs(GetWorld()->GetGravityZ()); //-980cm
    const float Time = (G > KINDA_SMALL_NUMBER) ? (2.f * JumpZ / G) : 0.8f;//y = x^2
    if (Time < 0.1f) return false;

    const float Vxy = Dist2D / Time;
    const FVector XY = Dir2D * Vxy;

    OutVel = FVector(XY.X, XY.Y, JumpZ);
    return true;
}


void AEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& /*UpdatedActors*/)
{
    UE_LOG(LogTemp, Warning, TEXT("OnPerceptionUpdated Fired!"));//関数ちゃんと使用するかどうか確認


    APawn* MyPawn = GetPawn();
    if (!MyPawn || !Perception) return;

	// 見れる全部のアクターを取得
    TArray<AActor*> SeenActors;
    Perception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), SeenActors);

    UE_LOG(LogTemp, Log, TEXT("Currently seeing %d actors."), SeenActors.Num());//見えてるアクター数

    ACPP_TVRPawn* Best = nullptr;
    float BestDistSq = TNumericLimits<float>::Max();

    for (AActor* A : SeenActors)
    {
        if (ACPP_TVRPawn* Player = Cast<ACPP_TVRPawn>(A))
        {
            // 距離
            const float D2 = FVector::DistSquared(Player->GetActorLocation(), MyPawn->GetActorLocation());
            if (D2 < BestDistSq)
            {
                BestDistSq = D2;
                Best = Player;
            }
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
        UE_LOG(LogTemp, Warning, TEXT("Found best target: %s"), *Best->GetName());
        SetFocus(Best);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No valid player target found in sight. Clearing target."));
        ClearFocus(EAIFocusPriority::Gameplay);
    }
}





// 2025.09.07 ウー start
FVector AEnemyAIController::GetNewPatrolLocation(float MinDistance)
{
    // Navのデータをゲット
    FNavAgentProperties NavAgentProps;
    ANavigationData* NavData = UNavigationSystemV1::GetCurrent(GetWorld())->GetNavDataForProps(NavAgentProps);

    // 
    if (NavData)
    {
        FNavLocation ResultLocation;
        // 今の世界にあるNavをゲット
        UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
        FVector MyLocation = GetPawn()->GetActorLocation();
        // MinDistanceが0だったら、制限なし
        float MinDist = MinDistance == 0.f ? 0.f : MinDistance;
        do
        {
            // Navの範囲外なら、今の座標を返す
            if (!IsActorOnNavMesh())
                return MyLocation;

            if (NavSystem)
            {
                NavSystem->GetRandomReachablePointInRadius(MyLocation, 10000.f, ResultLocation, NavData);
            }
        } while (FVector::DistXY(ResultLocation.Location, MyLocation) < MinDist);
        return ResultLocation.Location;
    }
    return GetPawn()->GetActorLocation();
}

bool AEnemyAIController::IsActorOnNavMesh()
{
    if (!GetPawn())
        return false;

    FVector ActorLocation = GetPawn()->GetActorLocation();

    // Navigation Systemを取得
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (!NavSys)
        return false;

    // Nav Mesh上の最も近い点を取得（MaxDistanceは適当に設定）
    FNavLocation NavLocation;
    bool bFound = NavSys->ProjectPointToNavigation(ActorLocation, NavLocation, FVector(100.0f, 100.0f, 300.0f));

    // bFoundがtrueならNav Mesh上に位置があることになる
    return bFound;
}
// 2025.09.07 ウー end
