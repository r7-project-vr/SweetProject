// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/CapsuleComponent.h>
#include <Camera/CameraComponent.h>
#include "TokumaruBranch/Actor/CPP_GetSpace.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "CPP_TVRPawn.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDamage, bool, damage);
UCLASS()
class SWEETPROJECT_API ACPP_TVRPawn : public APawn
{
	GENERATED_BODY()

	virtual float TakeDamage(
		float DamageAmount,
		const FDamageEvent& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;
public:
	// Sets default values for this pawn's properties
	ACPP_TVRPawn();

	/// <summary>
    /// しゃがみ判定になるための座標差
    /// </summary>
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
    /// ゲーム開始時のカメラの座標
    /// </summary>
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
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void EquipSword();

	/// <summary>
	/// 既に剣を装備しているかどうか
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Category = "Pickup")
	bool alreadyEquipSword = false;

	/// <summary>
	/// 剣を拾う場所からどれくらいの距離まで拾えるか
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Pickup")
	float pickupRange = 200.0f; // cm単位

	/// <summary>
	/// ゲーム開始時のカメラの高さを保存する
	/// </summary>
	void InitCameraPosition();

	/// <summary>
	/// 攻撃を受けた時の通知用
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FDamage damageDelegate;

	/// <summary>
	/// 攻撃を受けたかどうか
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool damageHit = false;

	/// <summary>
	/// ダメージを受けて何秒間怯むか
	/// </summary>
	UPROPERTY(EditAnywhere,Category = "damage")
	float stanSecend = 1.0f;

	/// <summary>
	/// 怯み解除タイマー数値減算用変数
	/// </summary>
	float currentStanSecond = 0.0f;

	/// <summary>
	/// コリジョンの立っている状態の高さ
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "heighter")
	float maxCollisionHeight = 160.0f;

	/// <summary>
	/// コリジョンのしゃがんでいる状態の高さ
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "heighter")
	float minCollisionHeight = 80.0f;

	/// <summary>
	/// カプセルコリジョンがシーンルートコンポーネントなので、従来のカプセルコリジョンの大きさと足元調整では、中心位置が変わるので、その変化分をカメラに適応させないようにする
	/// </summary>
	/// <param name="z"></param>
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CameraReset(float z);

	//UPROPERTY(EditAnywhere,Category = "heighter")
	//float characterWorldZ = 0.0f;

	/// <summary>
	/// 特定の壁を非表示にし、コリジョンを無効にする処理
	/// </summary>
	UFUNCTION()
	void HideBlockingWalls();

	/// <summary>
	/// 使用する入力マッピングコンテキスト
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* MyInputMappingContext;

	/// <summary>
	/// キャンディを拾うための入力アクション
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* PickUpCandyAction;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
