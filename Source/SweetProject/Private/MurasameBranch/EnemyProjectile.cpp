// Fill out your copyright notice in the Description page of Project Settings.


#include "MurasameBranch/EnemyProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemyProjectile::AEnemyProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    // Collision
    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    RootComponent = Collision;
    Collision->InitSphereRadius(8.f);
    Collision->SetCollisionProfileName(TEXT("Projectile"));
    Collision->SetNotifyRigidBodyCollision(true);

    // Mesh
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(Collision);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Movement
    Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
    Movement->InitialSpeed = 1800.f;
    Movement->MaxSpeed = 1800.f;
    Movement->bRotationFollowsVelocity = true;
    Movement->ProjectileGravityScale = 0.f;

	// Hit event
    Collision->OnComponentHit.AddDynamic(this, &AEnemyProjectile::OnHit);

    InitialLifeSpan = 5.f;
}

void AEnemyProjectile::OnHit(UPrimitiveComponent* /*HitComp*/, AActor* Other, UPrimitiveComponent* /*OtherComp*/,
    FVector /*NormalImpulse*/, const FHitResult& /*Hit*/)
{
    // 2025.09.18 ウー start
    //if (Other && Other != this)
    if (Other && Other->StaticClass() != this->StaticClass())
    {
        UGameplayStatics::ApplyDamage(
            Other, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
        Destroy();
    }
    // 2025.09.18 ウー end
}

// 2025.09.18 ウー start
void AEnemyProjectile::SetIgnoreActor(AActor* Actor)
{
	Collision->IgnoreActorWhenMoving(Actor, true);
}
// 2025.09.18 ウー end