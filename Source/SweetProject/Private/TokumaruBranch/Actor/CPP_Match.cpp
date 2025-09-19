// Fill out your copyright notice in the Description page of Project Settings.


#include "TokumaruBranch/Actor/CPP_Match.h"
// Sets default values
ACPP_Match::ACPP_Match()
	:
	countDown(matchFinishSecond)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MatchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MatchMesh"));
	// ルートにアタッチ
	MatchMesh->SetupAttachment(RootComponent);

	FireEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireEffect"));
	FireEffect->SetupAttachment(RootComponent);
	FireEffect->bAutoActivate = false;

	pLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("light"));
	pLight->SetupAttachment(RootComponent);

	FireNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FireNiagara"));
	FireNiagara->SetupAttachment(RootComponent);
	FireNiagara->bAutoActivate = false;

}

// Called when the game starts or when spawned
void ACPP_Match::BeginPlay()
{
	Super::BeginPlay();

	MatchMesh->SetVisibility(false);
	matchVisible = false;
	
}

// Called every frame
void ACPP_Match::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (FireNiagara) {
		FireNiagara->SetWorldRotation(FRotator(0,0,0));
	}

	if (isFire) {
		countDown -= (1 * DeltaTime);
		if (FireNiagara && MatchMesh) {
			//FireNiagara->SetVisibility(true);
			FireNiagara->SetRelativeScale3D(FVector(1, 1, 1));
			FireNiagara->SetRelativeScale3D(FireNiagara->GetRelativeScale3D() - FVector((1 / matchFinishSecond) * DeltaTime));

			if (normal) {
				if (countDown <= matchFinishSecond - ((matchFinishSecond / 3))) {
					FireNiagara->SetVisibility(true);
					changeMeshMaterial(1);
					koge = true;
					normal = false;
					marukoge = false;
				}
			}


			if (koge) {
				if (countDown <= matchFinishSecond - ((matchFinishSecond / 3) * 2)) {
					FireNiagara->SetVisibility(true);
					changeMeshMaterial(2);
					marukoge = true;
					koge = false;
					normal = false;
				}
			}

			
			//FVector zV = FireNiagara->GetRelativeLocation();
			//zV.Z -= (51.0f / 2) * (2.0f / 3.0f) / matchFinishSecond * DeltaTime;
			//FireNiagara->SetRelativeLocation(zV);
			//UE_LOG(LogTemp, Warning, TEXT("InitialCameraZ: %f, CurrentZ: %f, DeltaZ: %f"), a.X, a.Y, a.Z);
		}
		//if (MatchMesh) {
		//	FVector scale =  MatchMesh->GetRelativeScale3D();
		//	scale.Z -= 0.5 *(2.0f / 3.0f) /matchFinishSecond * DeltaTime;
		//	MatchMesh->SetRelativeScale3D(scale);
		//}


		if (countDown <= 0) {
			isFire = false;
			StopFire();
			countDown = matchFinishSecond;


			changeMeshMaterial(0);
			normal = true;
			marukoge = false;
			koge = false;
		}

	}
}

void ACPP_Match::StartFire()
{
	if (!matchVisible)return;
	if (FireNiagara)
	{
		//FireEffect->ActivateSystem();
		FireNiagara->ActivateSystem();
		isFire = true;
		CheckFireDelegate.Broadcast(isFire);
		pLight->SetVisibility(true);
		MatchMesh->SetVisibility(true);
	}
}

void ACPP_Match::StopFire()
{
	if (FireNiagara)
	{
		
		//FireEffect->DeactivateSystem();
		FireNiagara->Deactivate();
		isFire = false;
		CheckFireDelegate.Broadcast(isFire);

		FireNiagara->SetRelativeScale3D(FVector(1, 1, 1));
		//FireNiagara->SetRelativeLocation(FVector(FireNiagara->GetRelativeLocation().X, FireNiagara->GetRelativeLocation().Y, 23.0f));
		pLight->SetVisibility(false);
		MatchMesh->SetVisibility(false);
		//MatchMesh->SetRelativeScale3D(FVector(0.3f,0.3f,0.5f));

		matchVisible = false;
	}
}

void ACPP_Match::Interact(AActor* Interactor)
{
	if (!isFire) { 
		//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Cyan, TEXT("火がついてない"));
		return;
	}


	if (ICPP_UinterfaceToIntaract* interacter = Cast<ICPP_UinterfaceToIntaract>(Interactor)) {
		//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Cyan, TEXT("火を付けます"));
		interacter->BYInteract();
	}
}

void ACPP_Match::BYInteract()
{
	//CheckFireDelegate.AddDynamic(this, &ACPP_Match::test);
}

bool ACPP_Match::GetIsFire() const 
{
	return isFire;
}

void ACPP_Match::GetMatch()
{
	if (matchVisible)return;

	MatchMesh->SetVisibility(true);
	matchVisible = true;
}

