// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/Pillar.h"
#include "Components/BoxComponent.h"
#include <WuBranch/Actor/Match.h>

// Sets default values
APillar::APillar()
	: IsBurning(false)
	, BurningHPLossRate(0.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = Collision;

	PillarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pillar Mesh"));
	PillarMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APillar::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &APillar::OnCollisionOverlapBegin);
	Initialize();
}

void APillar::Initialize()
{
	HP = MaxHP;
	IsBurning = false;
}

// Called every frame
void APillar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsBurning)
	{
		Burning(DeltaTime);
	}
}

float APillar::GetHP() const
{
	return HP;
}

float APillar::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageToApply = FMath::Clamp(DamageToApply, 0.0f, HP);
	HP -= DamageToApply;

	// 破壊された
	if (HP <= 0.0f)
	{
		// エフェクト

		// エフェクトなどが終わったら消す
		Destroy();
	}
	else {
		// エフェクト

	}
	return 0.0f;
}

void APillar::OnCollisionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (AMatch* Weapon = Cast<AMatch>(OtherActor))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Pillar Hit!"));
			IsBurning = true;
			//// 攻撃者から攻撃力を貰う
			//float Attack = Weapon->GetAttackPower();
			//float DamageAmount = FMath::Clamp(Attack, 0.0f, HP); // 例として10のダメージを与える
			//HP -= DamageAmount;

			//// 破壊された
			//if (HP <= 0.0f)
			//{
			//	// エフェクト

			//	// エフェクトなどが終わったら消す
			//	Destroy();
			//}
			//else {
			//	// エフェクト

			//}
		}
	}
}

void APillar::Burning(float DeltaTime)
{
	float Damage = BurningHPLossRate * DeltaTime;
	HP -= Damage;

	FString txt = FString::Printf(TEXT("Pillar Burning! HP: %lf"), HP);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, txt);
	// 燃やされているエフェクト
	

	// 破壊された
	if (HP <= 0.0f)
	{
		// エフェクト

		// エフェクトなどが終わったら消す
		Destroy();
	}
}
