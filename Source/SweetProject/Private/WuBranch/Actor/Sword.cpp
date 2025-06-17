// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/Sword.h"
#include "Components/BoxComponent.h"

ASword::ASword()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	Collision->SetBoxExtent(FVector(10.0f, 10.0f, 10.0f));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void ASword::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
