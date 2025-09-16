// Fill out your copyright notice in the Description page of Project Settings.


#include "TokumaruBranch/Actor/CPP_EnterSpot.h"
#include "MurasameBranch/EnemyBase.h"
#include "Components/BoxComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ACPP_EnterSpot::ACPP_EnterSpot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_EnterSpot::OnOverlapBegin);
}

void ACPP_EnterSpot::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (searchEnemy)return;
    if (APawn* Pawn = Cast<APawn>(OtherActor)) {
        TArray<AActor*> Enemies;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyBase::StaticClass(), Enemies);

        for (AActor* Enemy : Enemies)
        {
            AEnemyBase* EnemyBase = Cast<AEnemyBase>(Enemy);
            if (EnemyBase)
            {
                EnemyBase->SetHighlighted(true, 1);
            }
            searchEnemy = true;
        }
    }
}


// Called when the game starts or when spawned
void ACPP_EnterSpot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_EnterSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

