// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MurasameBranch/EnemyStatsDA.h"
#include "EnemyBase.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;


UCLASS()
class SWEETPROJECT_API AEnemyBase : public ACharacter
{
    GENERATED_BODY()
public:
    AEnemyBase();

protected:
    virtual void BeginPlay() override;


    //9-10 AIPerceptionをEnemyBaseに追加
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
    UAIPerceptionComponent* Perception = nullptr;

    UPROPERTY()
    UAISenseConfig_Sight* SightCfg = nullptr;

    UFUNCTION() void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

    //9-10


public:
    // アセット
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    TObjectPtr<UEnemyStatsDA> Stats;

    //　いまのHP値
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    float CurrentHealth;

    // 死んでだ？
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsDead = false;

	// 傷害を受ける
    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
        AController* EventInstigator, AActor* DamageCauser) override;

    UFUNCTION(BlueprintCallable) bool IsAlive() const { return !bIsDead; }

	// 攻撃のAPI　近接攻撃と遠距離攻撃（リライト可能）
    UFUNCTION(BlueprintCallable) virtual void DoMeleeAttack(AActor* Target);
    UFUNCTION(BlueprintCallable) virtual void DoRangedAttack(AActor* Target);

	// 攻撃距離を取得（近と遠）
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent) float GetDesiredAttackRange() const;
    virtual float GetDesiredAttackRange_Implementation() const;

    // AI用引数
    FORCEINLINE float GetAttackInterval() const { return Stats ? Stats->AttackInterval : 1.5f; }
    FORCEINLINE float GetDamage() const { return Stats ? Stats->Damage : 10.f; }

    //Attack function
    virtual void Attack() { };

    // 2025.09.06 ウー start

    /// <summary>
    /// 今の移動スピードをゲット
    /// </summary>
    /// <returns></returns>
    float GetCurrentSpeed() const;

    /// <summary>
    /// 攻撃開始
    /// </summary>
    void StartAttack();

    /// <summary>
    /// 攻撃終了
    /// </summary>
    void CompleteAttack();

    /// <summary>
    /// 攻撃しているか
    /// </summary>
    /// <returns></returns>
    bool GetIsAttack() const;

    /// <summary>
    /// 死亡の通知
    /// </summary>
    /// <param name=""></param>
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeadDelegate);
    UPROPERTY(BlueprintAssignable)
    FDeadDelegate OnDeadEvent;

    //2025.09.16 得丸陽生
    virtual void SetHighlighted(bool enable, int number) {};
    //2025.09.16 得丸陽生 end

private:

    /// <summary>
    /// 死亡したのを通知する
    /// </summary>
    void NotifyDead();

    /// <summary>
    /// 攻撃のフラグ
    /// </summary>
    bool IsAttack;
    // 2025.09.06 ウー end
};
