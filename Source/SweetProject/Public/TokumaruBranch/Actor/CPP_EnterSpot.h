// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_EnterSpot.generated.h"

UCLASS()
class SWEETPROJECT_API ACPP_EnterSpot : public AActor
{
	GENERATED_BODY()

	bool searchEnemy = false;
	
public:	
	// Sets default values for this actor's properties
	ACPP_EnterSpot();


	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// <summary>
    /// コリジョン
    /// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Collision;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
