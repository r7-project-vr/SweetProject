// Fill out your copyright notice in the Description page of Project Settings.


#include "TokumaruBranch/Pawn/CPP_TVRPawn.h"
#include "TimerManager.h"
#include <Kismet/GameplayStatics.h>

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

	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPP_GetSpace::StaticClass(), foundActors);

	if (foundActors.Num() > 0)
	{
		swordPickupActor = foundActors[0]; // 条件に応じて選択可
	}

	//VROrigin = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("VROrigin")));
	MyVROrigin = FindComponentByClass<USceneComponent>();
	if (!MyVROrigin)
	{
		//VROrigin = FindComponentByClass<USceneComponent>(); 
		UE_LOG(LogTemp, Warning, TEXT("VROrigin not found"));
	}
	else {
		//UE_LOG(LogTemp, Warning, TEXT("VROrigin found!!!!!"));
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
		//UE_LOG(LogTemp, Warning, TEXT("Camera found!!!!!!!!!!!!"));
	}

	MyCapsuleComp = FindComponentByClass<UCapsuleComponent>();
	if (!MyCapsuleComp) {
		UE_LOG(LogTemp, Warning, TEXT("Capsule not found"));
	}
	else {
		//UE_LOG(LogTemp, Warning, TEXT("Capsule found!!!!!!!!!!!!"));
	}

	FTimerHandle resetCamerapos;
	GetWorldTimerManager().SetTimer(resetCamerapos, this, &ACPP_TVRPawn::InitCameraPosition, 0.1f, false);
}

// Called every frame
void ACPP_TVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MyCapsuleComp && MyCamera) {

		float CurrentZ = MyCamera->GetComponentLocation().Z;
		float DeltaZ = InitialCameraZ - CurrentZ;
		//UE_LOG(LogTemp, Warning, TEXT("InitialCameraZ: %f, CurrentZ: %f, DeltaZ: %f"), InitialCameraZ, CurrentZ, DeltaZ);
		if (DeltaZ > distanceToCrouching)
		{
			// カプセルの高さ変更
			if (!isCrouching) {
				SetCapsuleHeight(44.0f);// しゃがみ用サイズ
				isCrouching = true;
			}
			if (!alreadyEquipSword) {
				OnCrouchStart();
			}
		}
		else
		{
			if (isCrouching) {
				SetCapsuleHeight(88.0f); // 通常サイズ
				isCrouching = false;
			}
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

void ACPP_TVRPawn::OnCrouchStart()
{
	//UE_LOG(LogTemp, Warning, TEXT("しゃがんだら"));
	if (!swordPickupActor) {
		//UE_LOG(LogTemp, Warning, TEXT("ポイントがない！"));
		return;
	}

	//座標の間の距離を計算
	if (FVector::Dist(this->GetActorLocation(), swordPickupActor->GetActorLocation()) < pickupRange)
	{
		//UE_LOG(LogTemp, Warning, TEXT("装備する"));
		EquipSword();
	}
	else {
		//UE_LOG(LogTemp, Warning, TEXT("距離が足りない"));
	}
}

void ACPP_TVRPawn::InitCameraPosition()
{
	InitialCameraZ = MyCamera->GetComponentLocation().Z;
}

