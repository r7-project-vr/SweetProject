// Fill out your copyright notice in the Description page of Project Settings.


#include "TokumaruBranch/Actor/CPP_GetSpace.h"

// Sets default values
ACPP_GetSpace::ACPP_GetSpace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Spark = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Fire Effect"));
	Spark->SetupAttachment(RootComponent);
	Spark->bAutoActivate = false;

}

// Called when the game starts or when spawned
void ACPP_GetSpace::BeginPlay()
{
	Super::BeginPlay();

	GetComponents<UStaticMeshComponent>(MeshComponents);

}

// Called every frame
void ACPP_GetSpace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_GetSpace::FlashActor()
{
	if (hiddenActor)return;
	for (UStaticMeshComponent* MeshComp : MeshComponents)
	{
		if (!MeshComp)break;
		UE_LOG(LogTemp, Warning, TEXT("value"));
		MeshComp->SetRenderCustomDepth(true);
		MeshComp->CustomDepthStencilValue = 1;
		MeshComp->MarkRenderStateDirty();
	}
	Spark->ActivateSystem();
}

void ACPP_GetSpace::hidden()
{
	if (MeshComponents.Num() > 0) {
		for (UStaticMeshComponent* MeshComp : MeshComponents)
		{
			if (!MeshComp)break;
			MeshComp->SetHiddenInGame(true);
		}
	}
	hiddenActor = true;
	Spark->Deactivate();
}

