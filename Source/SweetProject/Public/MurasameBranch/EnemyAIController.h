// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "EnemyAIController.generated.h"

/**
 * 敵のAIコントローラー
 */
UCLASS()
class SWEETPROJECT_API AEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    AEnemyAIController();

    //ジャンプ用
    virtual void Tick(float DeltaSeconds) override;

protected:
    virtual void OnPossess(APawn* InPawn) override;

    /** 視覚 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
    TObjectPtr<UAIPerceptionComponent> Perception;

    /** 視覚のConfig */
    UPROPERTY()
    TObjectPtr<UAISenseConfig_Sight> SightCfg;

    /** 使ってるBT */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|BT")
    TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

    /** Create/Bind（UseBlackboardのOut引数） */
    UPROPERTY(Transient)
    TObjectPtr<UBlackboardComponent> BlackboardComp;

    /** BB Key名 */
    UPROPERTY(EditDefaultsOnly, Category = "AI|BB")
    FName KeyTarget = TEXT("TargetActor");

    UPROPERTY(EditDefaultsOnly, Category = "AI|BB")
    FName KeyDistance = TEXT("DistanceToTarget");

    UPROPERTY(EditDefaultsOnly, Category = "AI|BB")
    FName KeyTargetLoc = TEXT("TargetLocation");

    UPROPERTY(EditDefaultsOnly, Category = "AI|BB")
    FName KeyTargetAct = TEXT("TargetActor");

    /** Perceptionの更新及びcallback */
    UFUNCTION()
    void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

    // 2025.09.07 ウー start

public:

    /// <summary>
    /// 新しいパトロール位置をゲット
    /// </summary>
    /// <param name="MinDistance">最短距離</param>
    /// <returns>座標</returns>
    FVector GetNewPatrolLocation(float MinDistance = 0.0f);

    /// <summary>
    /// Navの範囲にいるか
    /// </summary>
    /// <returns>true: はい, false: いいえ</returns>
    bool IsActorOnNavMesh();
    // 2025.09.07 ウー end

    // 2025.09.08 ウー start
//private:
public:
    //Jump用
    void TickMeleeJump();

private:
    bool ComputeJumpVelocity(const FVector& From, const FVector& To,
        float JumpZ, float StepXY, FVector& OutVel) const;

    // 2025.09.07 ウー end
};