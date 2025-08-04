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

	UPROPERTY(EditAnywhere)
	float distanceToCrouching = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCapsuleComponent* MyCapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* MyVROrigin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* MyCamera;

	float InitialCameraZ = 0.0f;

	/// <summary>
	/// カプセルの高さの調整と、足元との違和感の払拭のために位置を調整する。
	/// </summary>
	/// <param name="newHeight"></param>
	UFUNCTION(BlueprintCallable)
	void SetCapsuleHeight(float newHeight);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
