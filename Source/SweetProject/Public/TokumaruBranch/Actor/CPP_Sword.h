// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Components/BoxComponent.h>
#include "TokumaruBranch/Interface/CPP_UinterfaceToIntaract.h"
#include "GameFramework/Actor.h"
#include "CPP_Sword.generated.h"

UCLASS()
class SWEETPROJECT_API ACPP_Sword : public AActor, public ICPP_UinterfaceToIntaract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Sword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AActor* Interactor) override;

	virtual void BYInteract() override;

	UPROPERTY(EditAnywhere)
	bool alreadyBaf = false;


	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* SwordMesh;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* AttackCollision;

	UFUNCTION(BlueprintCallable)
	void OnAttackCollisionBeginOverlap(AActor* OtherActor);

};
