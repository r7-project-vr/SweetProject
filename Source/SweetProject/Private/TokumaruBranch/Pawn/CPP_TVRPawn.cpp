// Fill out your copyright notice in the Description page of Project Settings.


#include "TokumaruBranch/Pawn/CPP_TVRPawn.h"

// Sets default values
ACPP_TVRPawn::ACPP_TVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	if (!CapsuleComp) {
		UE_LOG(LogTemp, Warning, TEXT("CapsuleCompNull"));
		return;
	}
	else {
		RootComponent = CapsuleComp;
	}

	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
	if (!VROrigin) {
		UE_LOG(LogTemp, Warning, TEXT("VROriginNull"));
		return;
	}
	else {
		VROrigin->SetupAttachment(RootComponent);
	}

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if (!Camera) {
		UE_LOG(LogTemp, Warning, TEXT("CameraNull"));
		return;
	}
	else {
		Camera->SetupAttachment(VROrigin);
	}


}

// Called when the game starts or when spawned
void ACPP_TVRPawn::BeginPlay()
{
	Super::BeginPlay();
	InitialCameraZ = Camera->GetRelativeLocation().Z;
}

// Called every frame
void ACPP_TVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CurrentZ = Camera->GetRelativeLocation().Z;
	float DeltaZ = InitialCameraZ - CurrentZ;

	if (DeltaZ > 30.0f) // 30cm
	{
		// カプセルの高さ変更
		CapsuleComp->SetCapsuleHalfHeight(44.0f); // しゃがみ用サイズ
	}
	else
	{
		CapsuleComp->SetCapsuleHalfHeight(88.0f); // 通常サイズ
	}
}

// Called to bind functionality to input
void ACPP_TVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

