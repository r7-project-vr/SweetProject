// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/Ceiling.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Components/BoxComponent.h"
#include "WuBranch/Actor/Pillar.h"

// Sets default values
ACeiling::ACeiling()
	: DisappearedPillarNum(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = Collision;

	Mesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Ceiling Mesh"));
	Mesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ACeiling::BeginPlay()
{
	Super::BeginPlay();
	

	DisappearedPillarNum = 0;
	// バインド
	for (APillar* Pillar : Pillars)
	{
		Pillar->OnDisappearNotify.AddDynamic(this, &ACeiling::OnPillarDisappear);
	}
	// 初期のオブジェクトタイプ
	Mesh->ObjectType = EObjectStateTypeEnum::Chaos_Object_Static;
}

void ACeiling::OnPillarDisappear()
{
	DisappearedPillarNum++;
	// 全部破壊されたか
	if (DisappearedPillarNum == Pillars.Num())
	{
		FallDown();
	}
}

void ACeiling::FallDown()
{
	Mesh->ObjectType = EObjectStateTypeEnum::Chaos_Object_Dynamic;
}

// Called every frame
//void ACeiling::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

