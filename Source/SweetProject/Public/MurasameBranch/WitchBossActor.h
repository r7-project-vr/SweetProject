// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WitchBossActor.generated.h"

UCLASS()
class SWEETPROJECT_API AWitchBossActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWitchBossActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* WitchMesh;

	//魔女顔向きZ軸のみか
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Witch|Facing")
	bool bYawOnly = false;

	//プレイヤActor指定
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Witch|Facing")
	TWeakObjectPtr<AActor> PlayerOverride;

	// Called when the game starts or when spawned
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable, Category = "Witch|Facing")
	void SetYawOnly(bool bInYawOnly) { bYawOnly = bInYawOnly; }

};
