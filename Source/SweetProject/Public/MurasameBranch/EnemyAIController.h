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

    /** Perceptionの更新及びcallback */
    UFUNCTION()
    void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
};