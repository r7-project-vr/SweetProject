// Fill out your copyright notice in the Description page of Project Settings.


#include "MurasameBranch/WitchBossActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AWitchBossActor::AWitchBossActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	WitchMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WitchMesh"));
	WitchMesh->SetupAttachment(Root);
	//モデルの向きに沿って調整する可能性ある


}

// Called when the game starts or when spawned
void AWitchBossActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWitchBossActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AActor* Player = PlayerOverride.Get();
	if (!Player)
	{
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			Player = PC->GetPawn();
		}
	}
	if (!IsValid(Player)) return;

	FVector WitchLoc = GetActorLocation();
	FVector TargetLoc = Player->GetActorLocation();

	if (bYawOnly)
	{
		WitchLoc.Z = 0.f;
		TargetLoc.Z = 0.f;
	}

	const FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(WitchLoc, TargetLoc);
	const FRotator NewRot = bYawOnly ? FRotator(0.f, LookAt.Yaw, 0.f) : LookAt;
	SetActorRotation(NewRot);



}

