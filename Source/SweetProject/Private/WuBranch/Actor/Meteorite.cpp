// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/Meteorite.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"

// Sets default values
AMeteorite::AMeteorite()
	: StartPoint(FVector::ZeroVector)
	, EndPoint(FVector::ZeroVector)
	, CanMove(false)
	, Speed(0.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));

	FireBallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireBall Mesh"));
	FireBallMesh->SetupAttachment(RootComponent);

	FireBallCollision = CreateDefaultSubobject<USphereComponent>(TEXT("FireBall Collision"));
	FireBallCollision->SetupAttachment(RootComponent);

	FireEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Fire Effect"));
	FireEffect->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMeteorite::BeginPlay()
{
	Super::BeginPlay();
	
	if (FireBallCollision)
		FireBallCollision->OnComponentBeginOverlap.AddDynamic(this, &AMeteorite::OnFireBallOverlapBegin);
	/*else
		UE_LOG(LogTemp, Error, TEXT("Bind BeginOverlap error"));*/
}

// Called every frame
void AMeteorite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);
	UpdateAttackRange();
}

void AMeteorite::SetTargetPoint(FVector Point)
{
	EndPoint = Point;
	MakeAttackRange();
}

void AMeteorite::Shoot()
{
	StartPoint = GetActorLocation();
	if ((int)FVector::Dist(StartPoint, EndPoint) != 0)
	{
		// 向き変更
		FVector Direction = EndPoint - StartPoint;
		SetActorRotation(Direction.Rotation());
		// 移動開始
		CanMove = true;
	}
}

void AMeteorite::SetTarget(TWeakObjectPtr<ACharacter> Target)
{
	CurrentTarget = Target;
}

void AMeteorite::SetAttackRangeRadius(float Radius)
{
	AttackRangeRadius = Radius;
}

void AMeteorite::OnFireBallOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
		return;

	CanMove = false;
	// 爆発エフェクト
	
	// ダメージ判定
	HandleDamage();
}

void AMeteorite::Move(float DeltaTime)
{
	if (CanMove)
	{
		FVector Direction = EndPoint - StartPoint;

		SetActorLocation(DeltaTime * Speed * Direction.GetSafeNormal() + GetActorLocation());
	}
}

void AMeteorite::MakeAttackRange()
{
	// マテリアル未設定
	if (!AttackRange)
		return;

	AttackRangeDynamic = UMaterialInstanceDynamic::Create(AttackRange, this);
	FVector HitNormal;
	FVector MaterialPoint = GetAttackRangeLocation(HitNormal);
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, FString::Printf(TEXT("attack effect: %s"), *MaterialPoint.ToString()));
	if (MaterialPoint != StartPoint)
	{
		AttackRangeDynamic->SetScalarParameterValue(TEXT("Percent"), 0);

		FVector Size = FVector(1, AttackRangeRadius, AttackRangeRadius);
		UDecalComponent* Hole = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), AttackRangeDynamic, Size, MaterialPoint, HitNormal.Rotation(), 0.f);
	}
}

FVector AMeteorite::GetAttackRangeLocation(FVector& OHitNormal)
{
	FHitResult HitResult;

	FVector Start = EndPoint + FVector::UpVector * 1000;
	FVector End = EndPoint + FVector::DownVector * 1000;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool Hit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
	if (Hit)
	{
		OHitNormal = HitResult.Normal;
		return HitResult.Location;
	}
	else
		return StartPoint;
}

void AMeteorite::UpdateAttackRange()
{
	if (CanMove && AttackRangeDynamic)
	{
		float Total = FVector::Dist(StartPoint, EndPoint);
		float Now = FVector::Dist(GetActorLocation(), EndPoint);

		float Percent =FMath::Clamp((Total - Now) / Total, 0, 1);
		AttackRangeDynamic->SetScalarParameterValue(TEXT("Percent"), Percent);
	}
}

void AMeteorite::HandleDamage()
{
	if (!CurrentTarget.IsValid())
		return;

	ACharacter* Target = CurrentTarget.Get();
	FVector MyLocation = GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();
	// 攻撃範囲外
	if ((TargetLocation - MyLocation).SizeSquared() > FMath::Square(AttackRangeRadius))
		return;

	// ダメージを与える
	UGameplayStatics::ApplyDamage(Target, 0.0f, nullptr, this, UDamageType::StaticClass());
}

void AMeteorite::NotifyDisappear()
{
	if (OnDisappearNotify.IsBound())
	{
		OnDisappearNotify.Broadcast();
	}
}