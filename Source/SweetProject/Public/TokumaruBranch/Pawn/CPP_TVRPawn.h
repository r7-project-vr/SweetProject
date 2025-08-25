// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/CapsuleComponent.h>
#include <Camera/CameraComponent.h>
#include "TokumaruBranch/Actor/CPP_GetSpace.h"
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

	UPROPERTY(EditAnywhere, Category = "Pickup")
	float distanceToCrouching = 0.0f;

	/// <summary>
    /// 剣取得範囲アクター
    /// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	AActor* swordPickupActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCapsuleComponent* MyCapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* MyVROrigin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* MyCamera;

	/// <summary>
    /// しゃがみ判定になるための座標差
    /// </summary>
	UPROPERTY(EditAnywhere, Category = "Pickup")
	float InitialCameraZ = 10.0f;

	/// <summary>
    /// しゃがんでるかどうか
    /// </summary>
	bool isCrouching = false;

	/// <summary>
	/// カプセルの高さの調整と、足元との違和感の払拭のために位置を調整する。
	/// </summary>
	/// <param name="newHeight"></param>
	UFUNCTION(BlueprintCallable)
	void SetCapsuleHeight(float newHeight);

	/// <summary>
    /// 剣の取得判定
    /// </summary>
	UFUNCTION(BlueprintCallable)
	void OnCrouchStart();

	/// <summary>
    /// 剣装備
    /// </summary>
	UFUNCTION(BlueprintImplementableEvent)
	void EquipSword();

	//すでに装備しているか
	UPROPERTY(BlueprintReadWrite, Category = "Pickup")
	bool alreadyEquipSword = false;

	// 判定距離
	UPROPERTY(EditAnywhere, Category = "Pickup")
	float pickupRange = 200.0f; // cm単位

	void InitCameraPosition();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
