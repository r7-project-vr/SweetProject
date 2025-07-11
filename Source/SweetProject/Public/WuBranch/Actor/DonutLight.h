// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DonutLight.generated.h"

class UBoxComponent;
class USpotLightComponent;

UCLASS()
class SWEETPROJECT_API ADonutLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADonutLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

private:

	/// <summary>
	/// 燃やせるものが近つく時
	/// </summary>
	UFUNCTION()
	void OnFireOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// <summary>
	/// ドーナツのメッシュ
	/// </summary>
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DonutMesh;

	/// <summary>
	/// ライトのメッシュ
	/// </summary>
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LightMesh;

	/// <summary>
	/// ドーナツのコリジョン
	/// </summary>
	UPROPERTY(EditAnywhere)
	UBoxComponent* Collision;

	/// <summary>
	/// ドーナツのライト
	/// </summary>
	UPROPERTY(EditAnywhere)
	USpotLightComponent* Light;

};
