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

		float CurrentZ = MyCamera->GetComponentLocation().Z;
		float DeltaZ = InitialCameraZ - CurrentZ;
		UE_LOG(LogTemp, Warning, TEXT("InitialCameraZ: %f, CurrentZ: %f, DeltaZ: %f"), InitialCameraZ, CurrentZ, DeltaZ);
		if (DeltaZ > distanceToCrouching)
		{
			// カプセルの高さ変更
			SetCapsuleHeight(44.0f);// しゃがみ用サイズ
			UE_LOG(LogTemp, Warning, TEXT("しゃがんでるよ"));
		}
		else
		{
			SetCapsuleHeight(88.0f); // 通常サイズ
			UE_LOG(LogTemp, Warning, TEXT("立ってるよ"));
		}
	}
}

// Called to bind functionality to input
void ACPP_TVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACPP_TVRPawn::SetCapsuleHeight(float newHeight)
{
	if (!MyCapsuleComp)return;

	//位置調整前のカプセルの半分の高さを取得
	float nowHalfHeight = MyCapsuleComp->GetUnscaledCapsuleHalfHeight();
	//調整先の高さと現在の高さの差を求める
	float nextHalfHeight = nowHalfHeight - newHeight;

	//カプセルの高さを更新
	MyCapsuleComp->SetCapsuleHalfHeight(newHeight, false);

	//調整前のカプセルの高さと調整後の高さの差からカプセルの中心位置を調整
	FVector currentCoupLoc = MyCapsuleComp->GetRelativeLocation();
	MyCapsuleComp->SetRelativeLocation(currentCoupLoc + FVector(0, 0, -nextHalfHeight));
}

