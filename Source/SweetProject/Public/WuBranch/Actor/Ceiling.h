// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ceiling.generated.h"

class UGeometryCollectionComponent;
class APillar;

UCLASS()
class SWEETPROJECT_API ACeiling : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACeiling();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

private:

	/// <summary>
	/// 柱が壊れた時の処理
	/// </summary>
	UFUNCTION()
	void OnPillarDisappear();

	/// <summary>
	/// 落下
	/// </summary>
	void FallDown();

	UPROPERTY(EditAnywhere)
	//UStaticMeshComponent* Mesh;
	UGeometryCollectionComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Collision;

	/// <summary>
	/// 支えている柱
	/// </summary>
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<APillar>> Pillars;

	/// <summary>
	/// 破壊された柱の数
	/// </summary>
	int DisappearedPillarNum;
};
