// Fill out your copyright notice in the Description page of Project Settings.


#include "TokumaruBranch/Actor/CPP_Sword.h"

// Sets default values
ACPP_Sword::ACPP_Sword()
	:
	currentSecond(powerUpLimitSecond),
	power(MaxPower)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MatchMesh"));
	// ルートにアタッチ
	SwordMesh->SetupAttachment(RootComponent);

	powerUpEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("powerUpEffect"));
	powerUpEffect->SetupAttachment(RootComponent);
	powerUpEffect->bAutoActivate = true;

	AttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcollision"));
	AttackCollision->SetupAttachment(SwordMesh);

}

// Called when the game starts or when spawned
void ACPP_Sword::BeginPlay()
{
	Super::BeginPlay();
	currentSecond = powerUpLimitSecond;
	//powerUpEffect->Deactivate();
}

// Called every frame
void ACPP_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//powerUpEffect->SetVisibility(true);

	if (alreadyBaf) {
		currentSecond -= 1 * DeltaTime;
		if (currentSecond <= 0) {
			power = MaxPower;
			alreadyBaf = false;
			currentSecond = powerUpLimitSecond;
			//if (powerUpEffect) {
			//	powerUpEffect->Deactivate();
			//}
		}
	}
}

void ACPP_Sword::Interact(AActor* Interactor)
{
}

void ACPP_Sword::BYInteract()
{
	if (alreadyBaf)return;
	alreadyBaf = true;
	power = MaxPower * 2;
	//if (powerUpEffect) {
	//	powerUpEffect->ActivateSystem();
	//	powerUpEffect->Activate(true);
	//}
}

void ACPP_Sword::OnCollisionBeginOverlapToMatch(AActor* OtherActor)
{
	if (!alreadyBaf) {
		if (OtherActor && OtherActor != this) {
			interacter->Interact(this);
		}
	}
}

bool ACPP_Sword::CheckMatch(AActor* OtherActor)
{
	if (OtherActor && OtherActor != this) {
		if (!interacter) {
			if (ICPP_UinterfaceToIntaract* inter = Cast<ICPP_UinterfaceToIntaract>(OtherActor)) {
				interacter = inter;
				return true;
			}
		}
		else if (ICPP_UinterfaceToIntaract* inter = Cast<ICPP_UinterfaceToIntaract>(OtherActor)) {
			return true;
		}
	}
	return false;
}

void ACPP_Sword::EffectVisivleReset()
{
	//powerUpEffect->SetVisibility(true);
	//powerUpEffect->Activate(true);
	//powerUpEffect->Deactivate();
}

