// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/Weapon.h"

// Sets default values
AWeapon::AWeapon()
	: AttackPower(10.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* myRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = myRoot;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

}

float AWeapon::GetAttackPower() const
{
	return AttackPower;
}

