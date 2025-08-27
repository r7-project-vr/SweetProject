// Fill out your copyright notice in the Description page of Project Settings.


#include "MurasameBranch/NavEnterSpawner.h"
#include "Components/BoxComponent.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"


// Sets default values
ANavEnterSpawner::ANavEnterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Box->InitBoxExtent(FVector(200.f));//Box大きさ調整

}

// Called when the game starts or when spawned
void ANavEnterSpawner::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &ANavEnterSpawner::OnBoxBeginOverlap);
}

void ANavEnterSpawner::OnBoxBeginOverlap(
	UPrimitiveComponent* /*OverlappedComponent*/,
	AActor* OtherActor,
	UPrimitiveComponent* /*OtherComp*/,
	int32 /*OtherBodyIndex*/,
	bool /*bFromSweep*/,
	const FHitResult& /*SweepResult*/)
{
	if (!IsValid(OtherActor)) return;

	//プレイヤPawnだけ反応ある
	const APawn* AsPawn = Cast<APawn>(OtherActor);
	if (!AsPawn) return;

	//地面スポット生成
	const FVector EndPoint = PickGroundPointNearPlayer(const_cast<AActor*>(OtherActor));
	//空中スポット生成
	const FVector StartPoint = PickAirPointUnderWitch();

	//未完成　DebugOutput
	UWorld* W = GetWorld();
	if (!W) return;

	DrawDebugSphere(W, EndPoint, 14.f, 12, FColor::Green, false, 6.f);
	DrawDebugSphere(W, StartPoint, 14.f, 12, FColor::Red, false, 6.f);
	DrawDebugLine(W, StartPoint, EndPoint, FColor::Yellow, false, 6.f, 0, 1.5f);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 4.0f, FColor::Cyan,
			FString::Printf(TEXT("Start(air): %s  End(ground): %s"),
				*StartPoint.ToCompactString(), *EndPoint.ToCompactString()));
	}
}

FVector ANavEnterSpawner::PickGroundPointNearPlayer(AActor* Player)
{
    const FVector Center = Player->GetActorLocation();

	// NavMesh使って、いける地点を探す
    if (bUseNavQuery)
    {
        if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
        {
            FNavLocation Out;
            //  GetRandomReachablePointInRadius　届ける地点
            if (NavSys->GetRandomReachablePointInRadius(Center, GroundRadius, Out))
            {
                return Out.Location;
            }
        }
    }

	//  地面に向けて線を引いて、ぶつかった地点を使う
    const float Angle = FMath::FRandRange(0.f, 2.f * PI);
    const float R = FMath::Sqrt(FMath::FRand()) * GroundRadius;
    FVector Offset(R * FMath::Cos(Angle), R * FMath::Sin(Angle), 0.f);
    FVector Test = Center + Offset + FVector(0, 0, 300.f);

    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(NavGroundTrace), false, Player);
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        Hit, Test, Test - FVector(0, 0, 2000.f), ECC_Visibility, Params);

	return bHit ? Hit.Location : (Center + Offset); // ぶつかっていないときはそのまま
}

static FVector RandomUnitVectorLowerHemisphere()
{
    // 下に向き単位ベクトル（Z <= 0）
    const float Z = -FMath::FRand();              // [-1,0]
    const float T = 2.f * PI * FMath::FRand();    // [0,2π)
    const float Rxy = FMath::Sqrt(1.f - Z * Z);
    return FVector(Rxy * FMath::Cos(T), Rxy * FMath::Sin(T), Z);
}

FVector ANavEnterSpawner::PickAirPointUnderWitch()
{
    const FVector Center = IsValid(WitchActor.Get()) ?
        WitchActor->GetActorLocation() : GetActorLocation();

    // 半径を立方根でスケーリング
    const float U = FMath::FRand();
	const float Radius = AirRadius * FMath::Pow(U, 1.f/3.f);
    const FVector Dir = RandomUnitVectorLowerHemisphere();
    return Center + Dir * Radius;
}