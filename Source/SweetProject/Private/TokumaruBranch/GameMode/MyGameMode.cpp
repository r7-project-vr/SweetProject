// Fill out your copyright notice in the Description page of Project Settings.


#include "TokumaruBranch/GameMode/MyGameMode.h"
// 2025.08.29 ウー start
#include "Kismet/GameplayStatics.h"
#include "WuBranch/Actor/Pillar.h"
// 2025.08.29 ウー end

AMyGameMode::AMyGameMode()
{
}

// 2025.08.29 ウー start
void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();

	DisappearedPillarNum = 0;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APillar::StaticClass(), Pillars);
	// バインド
	for (AActor* Actor : Pillars)
	{
		if(APillar* Pillar = Cast<APillar>(Actor))
			Pillar->OnDisappearNotify.AddDynamic(this, &AMyGameMode::OnPillarDisappear);
	}
}

void AMyGameMode::OnPillarDisappear()
{
	DisappearedPillarNum++;
	// 全部破壊されたか
	if (DisappearedPillarNum == Pillars.Num())
	{
		GameClear();
	}
}

void AMyGameMode::GameClear()
{

	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), LoadSuccLevel);
}

// 2025.08.29 ウー end