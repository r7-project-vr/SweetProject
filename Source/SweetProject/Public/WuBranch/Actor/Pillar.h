// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pillar.generated.h"

UCLASS()
class SWEETPROJECT_API APillar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APillar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Initialize();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	float GetHP() const;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

private:

	UFUNCTION()
	void OnCollisionOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// <summary>
	/// 焼かれている
	/// </summary>
	/// <param name="DeltaTime"></param>
	void Burning(float DeltaTime);

	/// <summary>
	/// メッシュ
	/// </summary>
	UPROPERTY(VisibleAnywhere, Category = "Pillar")
	UStaticMeshComponent* PillarMesh;

	/// <summary>
	/// コリジョン
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess="true"))
	class UBoxComponent* Collision;

	/// <summary>
	/// 最大HP
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Pillar", meta = (AllowPrivateAccess = "true"))
	float MaxHP;

	/// <summary>
	/// 今のHP
	/// </summary>
	UPROPERTY(VisibleAnywhere, Category = "Pillar", meta = (AllowPrivateAccess="true"))
	float HP;

	/// <summary>
	/// 焼かれている時HPが減る速度, HP / sec
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Pillar", meta = (AllowPrivateAccess = "true"))
	float BurningHPLossRate;

	/// <summary>
	/// 焼かれているか
	/// </summary>
	bool IsBurning;

};
