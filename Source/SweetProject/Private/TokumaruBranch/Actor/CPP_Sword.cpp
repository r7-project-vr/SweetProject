// Fill out your copyright notice in the Description page of Project Settings.


#include "TokumaruBranch/Actor/CPP_Sword.h"

// Sets default values
ACPP_Sword::ACPP_Sword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MatchMesh"));
	// ルートにアタッチ
	SwordMesh->SetupAttachment(RootComponent);

	AttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcollision"));
	AttackCollision->SetupAttachment(SwordMesh);

}

// Called when the game starts or when spawned
void ACPP_Sword::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_Sword::Interact(AActor* Interactor)
{
}

void ACPP_Sword::BYInteract()
{
	if (alreadyBaf)return;
	alreadyBaf = true;
	//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Cyan, TEXT("剣変更"));
	//UE_LOG(LogTemp, Error, TEXT("trueになったよ！！！！！！！！！！！！！！"));
}

void ACPP_Sword::OnAttackCollisionBeginOverlap(AActor* OtherActor)
{
	if (!alreadyBaf) {
		if (OtherActor && OtherActor != this) {
			if (ICPP_UinterfaceToIntaract* interacter = Cast<ICPP_UinterfaceToIntaract>(OtherActor)) {
				interacter->Interact(this);
			}
		}
	}
}

