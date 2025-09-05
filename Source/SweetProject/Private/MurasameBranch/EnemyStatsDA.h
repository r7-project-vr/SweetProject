// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyStatsDA.generated.h"

/**
 * 
 */
UCLASS()
class UEnemyStatsDA : public UPrimaryDataAsset
{
	GENERATED_BODY()


public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float MaxHealth = 100.f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float MoveSpeed = 300.f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float SightRadius = 2000.f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float LoseSightRadius = 2200.f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float SightFOV = 80.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly) float MeleeRange = 150.f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float RangedRange = 1200.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly) float AttackInterval = 1.5f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float Damage = 10.f;



};
