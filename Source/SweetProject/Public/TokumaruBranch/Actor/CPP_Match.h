// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Particles/ParticleSystemComponent.h"
#include "TokumaruBranch/Interface/CPP_UinterfaceToIntaract.h"
#include "GameFramework/Actor.h"
#include "CPP_Match.generated.h"

UCLASS()
class SWEETPROJECT_API ACPP_Match : public AActor,public ICPP_UinterfaceToIntaract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Match();

	UPROPERTY(BlueprintReadWrite)
	bool isFire = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "FX")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "FX")
	void StopFire();

	// 火のエフェクト
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FX")
	UParticleSystemComponent* FireEffect;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* MatchMesh;

	virtual void Interact(AActor* Interactor) override;

	virtual void BYInteract() override;


};
