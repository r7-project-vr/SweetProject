// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/CapsuleComponent.h>
#include <Camera/CameraComponent.h>
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CPP_TVRPawn.generated.h"

UCLASS()
class SWEETPROJECT_API ACPP_TVRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACPP_TVRPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* VROrigin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;

	float InitialCameraZ = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
