// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Components/BoxComponent.h>
#include "TokumaruBranch/Interface/CPP_UinterfaceToIntaract.h"
#include "GameFramework/Actor.h"
#include <NiagaraComponent.h>
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

	/// <summary>
	/// マッチによって既に強化されているか否か
	/// </summary>
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Power")
	bool alreadyBaf = false;


	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* SwordMesh;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* AttackCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FX")
	TObjectPtr<UNiagaraComponent> powerUpEffect;

	/// <summary>
	/// 攻撃力
	/// </summary>
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Power")
	float power = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Power")
	float MaxPower = 25.0f;

	float powerUpLimitSecond = 10.0f;

	float currentSecond;

	/// <summary>
	/// マッチに火がついている状態なら自信を強化する
	/// </summary>
	/// <param name="OtherActor"></param>
	UFUNCTION(BlueprintCallable)
	void OnCollisionBeginOverlapToMatch(AActor* OtherActor);

	/// <summary>
	/// 対象がマッチか確認する
	/// </summary>
	/// <param name="OtherActor"></param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable)
	bool CheckMatch(AActor* OtherActor);

	ICPP_UinterfaceToIntaract* interacter;

	UFUNCTION(BlueprintCallable)
	void EffectVisivleReset();

};
