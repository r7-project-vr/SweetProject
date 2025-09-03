// Fill out your copyright notice in the Description page of Project Settings.


#include "MurasameBranch/NavEnterSpawner.h"
// #include "Components/BoxComponent.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
// #include "TimerManager.h"


static FVector RandomUnitVectorLowerHemisphere()
{
	// 下に向き単位ベクトル（-1 <= Z <= 0）
	const float Z = -FMath::FRand();              // [-1,0]
	const float T = 2.f * PI * FMath::FRand();    // [0,2π)
	const float Rxy = FMath::Sqrt(1.f - Z * Z);
	return FVector(Rxy * FMath::Cos(T), Rxy * FMath::Sin(T), Z);
}


// Sets default values
ANavEnterSpawner::ANavEnterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	// Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	// SetRootComponent(Box);
	// Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	// Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	// Box->InitBoxExtent(FVector(200.f));//Box大きさ調整

}

// Called when the game starts or when spawned
void ANavEnterSpawner::BeginPlay()
{
	Super::BeginPlay();
	// Box->OnComponentBeginOverlap.AddDynamic(this, &ANavEnterSpawner::OnBoxBeginOverlap);
	// Box->OnComponentEndOverlap.AddDynamic(this, &ANavEnterSpawner::OnBoxEndOverlap);

	if (bAutoRunOnBeginPlay)
	{
		GenerateOnce();
	}
	
}


AActor* ANavEnterSpawner::ResolvePlayerActor() const
{
	UWorld* W = GetWorld();
	if (!W) return nullptr;


	// PlayerClassが指定されているとき、GetAllActorsOfClassで探す
	if (PlayerClass)
	{
		TArray<AActor*> Found;
		UGameplayStatics::GetAllActorsOfClass(this, PlayerClass, Found);
		if (Found.Num() > 0)
		{
			return Found[0];
		}
	}

	// 直接プレイヤーのPawnを取得
	if (APawn* P = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		return P;
	}

	// どちらも見つからないとき
	{
		TArray<AActor*> AnyPawns;
		UGameplayStatics::GetAllActorsOfClass(this, APawn::StaticClass(), AnyPawns);
		if (AnyPawns.Num() > 0)
		{
			return AnyPawns[0];
		}
	}

	return nullptr;


}


FVector ANavEnterSpawner::PickGroundPointNearPlayer(AActor* Player) const
{
	const FVector Center = Player->GetActorLocation();

	if (bUseNavQuery)
	{
		if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
		{
			FNavLocation Out;
			//　届ける点
			if (NavSys->GetRandomReachablePointInRadius(Center, GroundRadius, Out))
			{
				return Out.Location;
			}
		}
	}

	// NavMeshが使えないとき、地面に向けて線を引いて、ぶつかった地点を使う
	const float Angle = FMath::FRandRange(0.f, 2.f * PI);
	const float R = FMath::Sqrt(FMath::FRand()) * GroundRadius;
	const FVector Offset(R * FMath::Cos(Angle), R * FMath::Sin(Angle), 0.f);

	const FVector Start = Center + Offset + FVector(0, 0, 300.f);
	const FVector End = Start - FVector(0, 0, 2000.f);

	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(NavGroundTrace), false, Player);
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	return bHit ? Hit.Location : (Center + Offset);
}


FVector ANavEnterSpawner::PickAirPointUnderWitch() const
{
	const FVector Center = IsValid(WitchActor.Get()) ?
		WitchActor->GetActorLocation() : GetActorLocation();
	// 半径を立方根でスケーリング
	const float U = FMath::FRand();
	const float Radius = AirRadius * FMath::Pow(U, 1.f / 3.f);
	const FVector Dir = RandomUnitVectorLowerHemisphere();
	return Center + Dir * Radius;
}

void ANavEnterSpawner::GenerateOnce()
{
	UWorld* W = GetWorld();
	if (!W) return;

	AActor* Player = ResolvePlayerActor();
	if (!IsValid(Player))
	{
		UE_LOG(LogTemp, Warning, TEXT("[NavEnterSpawner] Player見つけていない、PlayerClass指定せよ"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 5.f, FColor::Red,
				TEXT("NavEnterSpawner: Player見つけていない（ PlayerClass または GetPlayerPawn(0)　チャックせよ）"));
		}
		return;
	}

	LastEndPoint = PickGroundPointNearPlayer(Player);
	LastStartPoint = PickAirPointUnderWitch();

	// DebugOutputテスト用
	DrawDebugSphere(W, LastStartPoint, 14.f, 12, FColor::Red, false, 6.f);
	DrawDebugSphere(W, LastEndPoint, 14.f, 12, FColor::Green, false, 12.f);
	DrawDebugLine(W, LastStartPoint, LastEndPoint, FColor::Yellow, false, 6.f, 0, 15.f);
	DrawDebugString(W, LastStartPoint + FVector(0, 0, 20),
		FString::Printf(TEXT("Start %s"), *LastStartPoint.ToCompactString()),
		nullptr, FColor::Red, 6.f, true);
	DrawDebugString(W, LastEndPoint + FVector(0, 0, 20),
		FString::Printf(TEXT("End %s"), *LastEndPoint.ToCompactString()),
		nullptr, FColor::Green, 6.f, true);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 4.f, FColor::Cyan,
			FString::Printf(TEXT("Start:%s  End:%s"),
				*LastStartPoint.ToCompactString(), *LastEndPoint.ToCompactString())
		);
	}
}

// 2025.08.30 ウー start
void ANavEnterSpawner::SetGroundRadius(float Radius)
{
	GroundRadius = Radius;
}

void ANavEnterSpawner::GetStartAndEndLocation(FVector& ONearWitch, FVector& ONearPlayer)
{
	UWorld* W = GetWorld();
	if (!W) return;

	AActor* Player = ResolvePlayerActor();
	if (!IsValid(Player))
	{
		UE_LOG(LogTemp, Warning, TEXT("[NavEnterSpawner] Player見つけていない、PlayerClass指定せよ"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 5.f, FColor::Red,
				TEXT("NavEnterSpawner: Player見つけていない（ PlayerClass または GetPlayerPawn(0)　チャックせよ）"));
		}
		return;
	}

	LastEndPoint = PickGroundPointNearPlayer(Player);
	LastStartPoint = PickAirPointUnderWitch();

	ONearWitch = LastStartPoint;
	ONearPlayer = LastEndPoint;

	// DebugOutputテスト用
	DrawDebugSphere(W, LastStartPoint, 14.f, 12, FColor::Red, false, 6.f);
	DrawDebugSphere(W, LastEndPoint, 14.f, 12, FColor::Green, false, 6.f);
	DrawDebugLine(W, LastStartPoint, LastEndPoint, FColor::Yellow, false, 6.f, 0, 1.5f);
	DrawDebugString(W, LastStartPoint + FVector(0, 0, 20),
		FString::Printf(TEXT("Start %s"), *LastStartPoint.ToCompactString()),
		nullptr, FColor::Red, 6.f, true);
	DrawDebugString(W, LastEndPoint + FVector(0, 0, 20),
		FString::Printf(TEXT("End %s"), *LastEndPoint.ToCompactString()),
		nullptr, FColor::Green, 6.f, true);


	GEngine->AddOnScreenDebugMessage(
		-1, 4.f, FColor::Cyan,
		FString::Printf(TEXT("Start:%s  End:%s"),
		*LastStartPoint.ToCompactString(), *LastEndPoint.ToCompactString())
	);
}
// 2025.08.30 ウー end

// Version 1.1
// void ANavEnterSpawner::OnBoxBeginOverlap(
// 	UPrimitiveComponent* /*OverlappedComponent*/,
// 	AActor* OtherActor,
// 	UPrimitiveComponent* /*OtherComp*/,
// 	int32 /*OtherBodyIndex*/,
// 	bool /*bFromSweep*/,
// 	const FHitResult& /*SweepResult*/)
// {
// 	APawn* Pawn = Cast<APawn>(OtherActor);
// 	if (!Pawn) return;
//
// 	//プレイヤーを追跡、ループ開始
// 	TrackedPawn = Pawn;
// 	StartSpawningLoop(Pawn);
//
// 	//if (!IsValid(OtherActor)) return;
//
// 	////プレイヤPawnだけに反応ある
// 	//const APawn* AsPawn = Cast<APawn>(OtherActor);
// 	//if (!AsPawn) return;
//
// 	////地面スポット生成
// 	//const FVector EndPoint = PickGroundPointNearPlayer(const_cast<AActor*>(OtherActor));
// 	////空中スポット生成
// 	//const FVector StartPoint = PickAirPointUnderWitch();
//
// 	////未完成　DebugOutput
// 	//UWorld* W = GetWorld();
// 	//if (!W) return;
//
// 	//DrawDebugSphere(W, EndPoint, 14.f, 12, FColor::Green, false, 6.f);
// 	//DrawDebugSphere(W, StartPoint, 14.f, 12, FColor::Red, false, 6.f);
// 	//DrawDebugLine(W, StartPoint, EndPoint, FColor::Yellow, false, 6.f, 0, 1.5f);
//
// 	//if (GEngine)
// 	//{
// 	//	GEngine->AddOnScreenDebugMessage(
// 	//		-1, 4.0f, FColor::Cyan,
// 	//		FString::Printf(TEXT("Start(air): %s  End(ground): %s"),
// 	//			*StartPoint.ToCompactString(), *EndPoint.ToCompactString()));
// 	//}
//
// }
//
// void ANavEnterSpawner::OnBoxEndOverlap(
// 	UPrimitiveComponent*,
// 	AActor* OtherActor,
// 	UPrimitiveComponent*,
// 	int32) 
// {
// 	APawn* Pawn = Cast<APawn>(OtherActor);
// 	if (!Pawn) return;
//
// 	//PawnがTriggerBoxにありませんとき、ループ停止
// 	TArray<AActor*> Overlapping;
// 	Box->GetOverlappingActors(Overlapping, APawn::StaticClass());
// 	if (Overlapping.Num() == 0)
// 	{
// 		TrackedPawn = nullptr;
// 		StopSpawningLoop();
// 	}
// 	else
// 	{
// 		//もし追跡のPawnが離れたら、追跡させるPawnを変わり
// 		if (TrackedPawn.Get() == Pawn)
// 		{
// 			for (AActor* A : Overlapping)
// 			{
// 				if (APawn* P = Cast<APawn>(A))
// 				{
// 					TrackedPawn = P;
// 					break;
// 				}
// 			}
// 		}
// 	}
// }
//
//
// void ANavEnterSpawner::StartSpawningLoop(APawn* /*Pawn*/)
// {
// 	if (!GetWorld()) return;
//
// 	//二重発動しない
// 	if (GetWorldTimerManager().IsTimerActive(SpawnTimerHandle)) return;
//
// 	//タイマーループで使う
// 	GetWorldTimerManager().SetTimer(
// 		SpawnTimerHandle, this, &ANavEnterSpawner::SpawnOnce,
// 		SpawnInterval, true, FirstDelay
// 	);
//
// 	//FirstDelay = 0.0fのときがすぐ生成
// 	//または、SpawnOnce()を使用
//
// }
//
//
// void ANavEnterSpawner::StopSpawningLoop()
// {
// 	if (GetWorld())
// 	{
// 		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
// 	}
// }
//
// void ANavEnterSpawner::SpawnOnce()
// {
// 	//Pawnなしー＞TimerStop
// 	if (bStopIfNoPawnInside)
// 	{
// 		bool bAnyPawnInside = Box->IsOverlappingActor(TrackedPawn.Get());
// 		if (!bAnyPawnInside)
// 		{
// 			//他のPawnあるか　確認
// 			TArray<AActor*> Overlapping;
// 			Box->GetOverlappingActors(Overlapping, APawn::StaticClass());
// 			if (Overlapping.Num() == 0)
// 			{
// 				StopSpawningLoop();
// 				return;
// 			}
// 			else
// 			{
// 				TrackedPawn = Cast<APawn>(Overlapping[0]);
// 			}
// 		}
// 	}
//
// 	AActor* Player = TrackedPawn.Get();
// 	if (!IsValid(Player))
// 	{
// 		StopSpawningLoop();
// 		return;
// 	}
//
// 	const FVector EndPoint = PickGroundPointNearPlayer(Player);
// 	const FVector StartPoint = PickAirPointUnderWitch();
//
// 	//DebugOutputテスト用
// 	DrawDebugSphere(GetWorld(), StartPoint, 14.f, 12, FColor::Red, false,
// 		SpawnInterval * 1.5f);
// 	DrawDebugSphere(GetWorld(), EndPoint, 14.f, 12, FColor::Green, false,
// 		SpawnInterval * 1.5f);
// 	DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Yellow, false,
// 		SpawnInterval * 1.5f, 0, 1.5f);
// 	DrawDebugString(GetWorld(), StartPoint + FVector(0, 0, 20),
// 		FString::Printf(TEXT("Start %s"), *StartPoint.ToCompactString()),
// 		nullptr, FColor::Red, SpawnInterval * 1.5f, true);
// 	DrawDebugString(GetWorld(), EndPoint + FVector(0, 0, 20),
// 		FString::Printf(TEXT("End %s"), *EndPoint.ToCompactString()),
// 		nullptr, FColor::Green, SpawnInterval * 1.5f, true);
//
// 	if (GEngine)
// 	{
// 		GEngine->AddOnScreenDebugMessage(
// 			0, SpawnInterval * 0.9f, FColor::Cyan,
// 			FString::Printf(TEXT("Start:%s End:%s"),
// 				*StartPoint.ToCompactString(), *EndPoint.ToCompactString())
// 		);
// 	}
//
// }
//
//
// FVector ANavEnterSpawner::PickGroundPointNearPlayer(AActor* Player)
// {
//     const FVector Center = Player->GetActorLocation();
//
// 	// NavMesh使って、いける地点を探す
//     if (bUseNavQuery)
//     {
//         if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
//         {
//             FNavLocation Out;
//             //  GetRandomReachablePointInRadius　届ける地点
//             if (NavSys->GetRandomReachablePointInRadius(Center, GroundRadius, Out))
//             {
//                 return Out.Location;
//             }
//         }
//     }
//
// 	//  地面に向けて線を引いて、ぶつかった地点を使う
//     const float Angle = FMath::FRandRange(0.f, 2.f * PI);
//     const float R = FMath::Sqrt(FMath::FRand()) * GroundRadius;
//     FVector Offset(R * FMath::Cos(Angle), R * FMath::Sin(Angle), 0.f);
//     FVector Test = Center + Offset + FVector(0, 0, 300.f);
//
//     FHitResult Hit;
//     FCollisionQueryParams Params(SCENE_QUERY_STAT(NavGroundTrace), false, Player);
//     bool bHit = GetWorld()->LineTraceSingleByChannel(
//         Hit, Test, Test - FVector(0, 0, 2000.f), ECC_Visibility, Params);
//
// 	return bHit ? Hit.Location : (Center + Offset); // ぶつかっていないときはそのまま
// }
//
// static FVector RandomUnitVectorLowerHemisphere()
// {
//     // 下に向き単位ベクトル（Z <= 0）
//     const float Z = -FMath::FRand();              // [-1,0]
//     const float T = 2.f * PI * FMath::FRand();    // [0,2π)
//     const float Rxy = FMath::Sqrt(1.f - Z * Z);
//     return FVector(Rxy * FMath::Cos(T), Rxy * FMath::Sin(T), Z);
// }
//
// FVector ANavEnterSpawner::PickAirPointUnderWitch()
// {
//     const FVector Center = IsValid(WitchActor.Get()) ?
//         WitchActor->GetActorLocation() : GetActorLocation();
//
//     // 半径を立方根でスケーリング
//     const float U = FMath::FRand();
// 	const float Radius = AirRadius * FMath::Pow(U, 1.f/3.f);
//     const FVector Dir = RandomUnitVectorLowerHemisphere();
//     return Center + Dir * Radius;
// }