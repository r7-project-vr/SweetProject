// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/Match.h"
#include "Components/BoxComponent.h"

AMatch::AMatch()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	Collision->SetBoxExtent(FVector(10.0f, 10.0f, 10.0f));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AMatch::BeginPlay()
{
	Super::BeginPlay();

	
}

void AMatch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}
