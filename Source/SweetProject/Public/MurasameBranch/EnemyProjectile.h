// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

#include "EnemyProjectile.generated.h"

UCLASS()
class SWEETPROJECT_API AEnemyProjectile : public AActor
{
	GENERATED_BODY()
public:
	AEnemyProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	USphereComponent* Collision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UProjectileMovementComponent* Movement = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float Damage = 10.f;

protected:
	UFUNCTION() void OnHit(UPrimitiveComponent* HitComp,
		AActor* Other,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);
};
