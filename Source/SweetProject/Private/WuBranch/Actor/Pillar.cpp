// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/Pillar.h"
#include "Components/BoxComponent.h"
#include <WuBranch/Actor/Match.h>

// Sets default values
APillar::APillar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	USceneComponent* myRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = myRoot;

	PillarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pillar Mesh"));
	PillarMesh->SetupAttachment(RootComponent);
	
	/*Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &APillar::OnCollisionOverlapBegin);*/
}

// Called when the game starts or when spawned
void APillar::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
	PillarMesh->OnComponentBeginOverlap.AddDynamic(this, &APillar::OnCollisionOverlapBegin);
}

void APillar::Initialize()
{
	HP = MaxHP;
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
			// 攻撃者から攻撃力を貰う
			float Attack = Weapon->GetAttackPower();
			float DamageAmount = FMath::Clamp(Attack, 0.0f, HP); // 例として10のダメージを与える
			HP -= DamageAmount;

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
		}
	}
}

// Called every frame
//void APillar::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

