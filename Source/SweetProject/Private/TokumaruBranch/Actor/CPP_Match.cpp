// Fill out your copyright notice in the Description page of Project Settings.


#include "TokumaruBranch/Actor/CPP_Match.h"

// Sets default values
ACPP_Match::ACPP_Match()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MatchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MatchMesh"));
	// ルートにアタッチ
	MatchMesh->SetupAttachment(RootComponent);

	FireEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireEffect"));
	FireEffect->SetupAttachment(RootComponent);
	FireEffect->bAutoActivate = false;

}

// Called when the game starts or when spawned
void ACPP_Match::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_Match::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_Match::StartFire()
{
	if (FireEffect)
	{
		FireEffect->ActivateSystem();
		isFire = true;
	}
}

void ACPP_Match::StopFire()
{
	if (FireEffect)
	{
		FireEffect->DeactivateSystem();
		isFire = false;
	}
}

void ACPP_Match::Interact(AActor* Interactor)
{
	if (!isFire) { 
		//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Cyan, TEXT("火がついてない"));
		return;
	}


	if (ICPP_UinterfaceToIntaract* interacter = Cast<ICPP_UinterfaceToIntaract>(Interactor)) {
		//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Cyan, TEXT("火を付けます"));
		interacter->BYInteract();
	}
}

void ACPP_Match::BYInteract()
{
}

