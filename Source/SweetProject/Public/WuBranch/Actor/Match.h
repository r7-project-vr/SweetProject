// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WuBranch/Actor/Weapon.h"
#include "Match.generated.h"

/**
 * 
 */
UCLASS()
class SWEETPROJECT_API AMatch : public AWeapon
{
	GENERATED_BODY()

public:

	AMatch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Collision;
	
};
