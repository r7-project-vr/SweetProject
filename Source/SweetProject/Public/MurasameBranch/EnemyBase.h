// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UCLASS()
class SWEETPROJECT_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float AttackRange = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float AttackInterval = 2.0f;

	UFUNCTION(BlueprintCallable, Category = "AI")
	bool CanAttack() const;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void MarkJustAttacked();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	float LastAttackTime = -FLT_MAX;

};
