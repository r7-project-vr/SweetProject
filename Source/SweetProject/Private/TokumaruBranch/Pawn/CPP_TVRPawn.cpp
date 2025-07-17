// Fill out your copyright notice in the Description page of Project Settings.


#include "TokumaruBranch/Pawn/CPP_TVRPawn.h"

// Sets default values
ACPP_TVRPawn::ACPP_TVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void ACPP_TVRPawn::BeginPlay()
{
	Super::BeginPlay();
	//VROrigin = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("VROrigin")));
	MyVROrigin = FindComponentByClass<USceneComponent>();
	if (!MyVROrigin)
	{
		//VROrigin = FindComponentByClass<USceneComponent>(); 
		UE_LOG(LogTemp, Warning, TEXT("VROrigin not found"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("VROrigin found!!!!!"));
	}

	//Camera = Cast<UCameraComponent>(GetDefaultSubobjectByName(TEXT("Camera")));
	MyCamera = FindComponentByClass<UCameraComponent>();
	if (!MyCamera)
	{
		//Camera = FindComponentByClass<UCameraComponent>();
		UE_LOG(LogTemp, Warning, TEXT("Camera not found"));
	}
	if (MyCamera)
	{
		InitialCameraZ = MyCamera->GetComponentLocation().Z;
		UE_LOG(LogTemp, Warning, TEXT("Camera found!!!!!!!!!!!!"));
	}

	MyCapsuleComp = FindComponentByClass<UCapsuleComponent>();
	if (!MyCapsuleComp) {
		UE_LOG(LogTemp, Warning, TEXT("Capsule not found"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Capsule found!!!!!!!!!!!!"));
	}
}

// Called every frame
void ACPP_TVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MyCapsuleComp && MyCamera) {

		float CurrentZ = MyCamera->GetRelativeLocation().Z;
		float DeltaZ = InitialCameraZ - CurrentZ;
		UE_LOG(LogTemp, Warning, TEXT("InitialCameraZ: %f, CurrentZ: %f, DeltaZ: %f"), InitialCameraZ, CurrentZ, DeltaZ);
		if (DeltaZ > 10.0f && CurrentCapsuleHeight != 44.0f) // 30cm
		{
			// カプセルの高さ変更
			MyCapsuleComp->SetCapsuleHalfHeight(44.0f); // しゃがみ用サイズ
			CurrentCapsuleHeight = 44.0f;
			UE_LOG(LogTemp, Warning, TEXT("しゃがんでるよ"));
		}
		else
		{
			MyCapsuleComp->SetCapsuleHalfHeight(88.0f); // 通常サイズ
			CurrentCapsuleHeight = 88.0f;
			UE_LOG(LogTemp, Warning, TEXT("立ってるよ"));
		}
	}
}

// Called to bind functionality to input
void ACPP_TVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

