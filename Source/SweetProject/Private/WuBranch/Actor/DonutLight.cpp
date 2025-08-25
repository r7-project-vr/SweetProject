// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/DonutLight.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PointLightComponent.h"
#include "WuBranch/Actor/Weapon.h"

// Sets default values
ADonutLight::ADonutLight()
	: MaxIntensity(50.f)
	, CurrentState(EDonutLightStates::DonutLightOff)
	, GlowDuration(0.f)
	, TimeCounter(0.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	DonutMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DonutMesh"));
	DonutMesh->SetupAttachment(RootComponent);

	LightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightMesh"));
	LightMesh->SetupAttachment(RootComponent);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(DonutMesh);
	
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	Light->SetupAttachment(LightMesh);
	Light->SetIntensityUnits(ELightUnits::Lumens);
}

// Called when the game starts or when spawned
void ADonutLight::BeginPlay()
{
	Super::BeginPlay();
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ADonutLight::OnFireOverlapBegin);
	// 初期状態ではライトを非表示にする
	TurnOffLight();
}

// Called every frame
void ADonutLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GlowDuration > 0.0f && CurrentState == EDonutLightStates::DonutLightOn)
	{
		TimeCounter -= DeltaTime;
		UpdateLightIntensity(TimeCounter);
		if(TimeCounter <= 0.0f)
		{
			// 時間が経過したらライトを消す
			TurnOffLight();
		}
	}
}

void ADonutLight::OnFireOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		// 燃やせるものが近づいたら、ライトを点灯させる
		if (OtherActor->IsA(AWeapon::StaticClass()))
		{
			TurnOnLight();
		}
	}
}

void ADonutLight::UpdateLightIntensity(float Time)
{
	float Percent = Time / GlowDuration;
	Light->SetIntensity(FMath::Clamp(MaxIntensity * Percent, 0.f, MaxIntensity));
}

void ADonutLight::TurnOnLight()
{
	// 既にライトが点灯している場合は何もしない
	if (CurrentState == EDonutLightStates::DonutLightOn)
		return;

	// ライトの強度を最大に設定
	Light->SetIntensity(MaxIntensity);
	CurrentState = EDonutLightStates::DonutLightOn;
	if (GlowDuration > 0.f)
	{
		TimeCounter = GlowDuration;
	}
}

void ADonutLight::TurnOffLight()
{
	Light->SetIntensity(0.f);
	CurrentState = EDonutLightStates::DonutLightOff;
}

