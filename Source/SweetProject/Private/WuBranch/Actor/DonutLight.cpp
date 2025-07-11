// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/DonutLight.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SpotLightComponent.h"
#include "WuBranch/Actor/Weapon.h"

// Sets default values
ADonutLight::ADonutLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	DonutMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DonutMesh"));
	DonutMesh->SetupAttachment(RootComponent);

	LightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightMesh"));
	LightMesh->SetupAttachment(RootComponent);

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));	
	Collision->SetupAttachment(RootComponent);
	
	Light = CreateDefaultSubobject<USpotLightComponent>(TEXT("Light"));
	Light->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ADonutLight::BeginPlay()
{
	Super::BeginPlay();
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ADonutLight::OnFireOverlapBegin);
}

// Called every frame
//void ADonutLight::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void ADonutLight::OnFireOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		// 燃やせるものが近づいたら、ライトを点灯させる
		if (OtherActor->IsA(AWeapon::StaticClass()))
		{
			Light->SetVisibility(true);
			UE_LOG(LogTemp, Warning, TEXT("Donut Light Activated!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Non-flammable object detected."));
		}
	}
}

