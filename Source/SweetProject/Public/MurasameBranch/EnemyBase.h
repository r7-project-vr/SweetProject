// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MurasameBranch/EnemyStatsDA.h"
#include "EnemyBase.generated.h"

UCLASS()
class SWEETPROJECT_API AEnemyBase : public ACharacter
{
    GENERATED_BODY()
public:
    AEnemyBase();

protected:
    virtual void BeginPlay() override;

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
};
