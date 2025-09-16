// Fill out your copyright notice in the Description page of Project Settings.



#include "WuBranch/Actor/Pillar.h"
//2025.08.29 得丸陽生
#include "TokumaruBranch/Actor/CPP_Match.h"
//2025.08.29 得丸陽生 end
#include "Components/BoxComponent.h"
#include <WuBranch/Actor/Match.h>
#include "NiagaraComponent.h"

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

	FireEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Fire Effect"));
	FireEffect->SetupAttachment(RootComponent);
	FireEffect->bAutoActivate = false;
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
	//2025 09.16 得丸陽生
	//if (FireEffect)
	//{
	//	FireEffect->Activate(false);
	//}
	//2025 09.16 得丸陽生
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

void APillar::OnCollisionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{

		//2025.08.29 得丸陽生
		if (ACPP_Match* MatchActor = Cast<ACPP_Match>(OtherActor)) {
			if (MatchActor->GetIsFire()) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Pillar Hit!"));
				IsBurning = true;
			}
		}

		//if (AMatch* Weapon = Cast<AMatch>(OtherActor))
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Pillar Hit!"));
		//	IsBurning = true;
		//	//// 攻撃者から攻撃力を貰う
		//	//float Attack = Weapon->GetAttackPower();
		//	//float DamageAmount = FMath::Clamp(Attack, 0.0f, HP); // 例として10のダメージを与える
		//	//HP -= DamageAmount;

		//	//// 破壊された
		//	//if (HP <= 0.0f)
		//	//{
		//	//	// エフェクト

		//	//	// エフェクトなどが終わったら消す
		//	//	Destroy();
		//	//}
		//	//else {
		//	//	// エフェクト

		//	//}
		//}
		//2025.08.29 得丸陽生 end
	}
}

void APillar::Burning(float DeltaTime)
{
	float Damage = BurningHPLossRate * DeltaTime;
	HP -= Damage;

	FString txt = FString::Printf(TEXT("Pillar Burning! HP: %lf"), HP);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, txt);
	// 燃やされているエフェクト
	if (FireEffect)
	{

		//2025.09.02 得丸陽生
		if (!FireEffect->IsActive()) {
			FireEffect->Activate(true);
		}

		FireEffect->SetRelativeLocation(FireEffect->GetRelativeLocation() + (FVector(0.0f,0.0f, (200.0f / (HP / BurningHPLossRate)) * DeltaTime)));
		//2025.09.02 得丸陽生 end
	}

	// 破壊された
	if (HP <= 0.0f)
	{
		NotifyDisappear();
		// エフェクト

		// エフェクトなどが終わったら消す
		Destroy();
	}
}

void APillar::NotifyDisappear()
{
	if (OnDisappearNotify.IsBound())
		OnDisappearNotify.Broadcast();
}
