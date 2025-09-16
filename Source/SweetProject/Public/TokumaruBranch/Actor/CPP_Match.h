// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Particles/ParticleSystemComponent.h"
#include "TokumaruBranch/Interface/CPP_UinterfaceToIntaract.h"
#include <Components/PointLightComponent.h>
#include "GameFramework/Actor.h"
#include <NiagaraComponent.h>
#include "CPP_Match.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFire, bool,fire);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateCoinDelegate, int, Num);

UCLASS()
class SWEETPROJECT_API ACPP_Match : public AActor,public ICPP_UinterfaceToIntaract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Match();

	/// <summary>
	/// 火がついてるかどうか
	/// </summary>
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FX")
	TObjectPtr<UNiagaraComponent> FireNiagara;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Mesh")
	UStaticMeshComponent* MatchMesh;

	UPROPERTY(VisibleAnywhere, Category = "Light")
	UPointLightComponent* pLight;

	virtual void Interact(AActor* Interactor) override;

	virtual void BYInteract() override;

	/// <summary>
	/// 火の状態更新を見る
	/// </summary>
	UPROPERTY(BlueprintAssignable,Category = "Events")
	FFire CheckFireDelegate;


	/// <summary>
	/// 今の火の状態を取得
	/// </summary>
	/// <returns></returns>
	bool GetIsFire() const;

	/// <summary>
	/// マッチを取得
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void GetMatch();

	/// <summary>
	/// マッチが何秒で消えるか
	/// </summary>
	UPROPERTY(EditAnywhere)
	float matchFinishSecond = 15.0f;

	/// <summary>
	/// マッチ消火タイマー数値減算用変数
	/// </summary>
	float countDown = 0.0f;


	/// <summary>
	/// マッチを持ってるかどうか
	/// </summary>
	UPROPERTY(BlueprintReadWrite)
	bool matchVisible = true;

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void changeMeshMaterial(int state);

	bool normal = true;
	bool koge = false;
	bool marukoge = false;

};