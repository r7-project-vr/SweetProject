// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/Meteorite.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"
#include "MurasameBranch/WitchBossActor.h"

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

	ExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Explosion Effect"));
	ExplosionEffect->SetupAttachment(RootComponent);
	ExplosionEffect->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AMeteorite::BeginPlay()
{
	Super::BeginPlay();
	
	AttackRangeDynamic = nullptr;

	if (FireBallCollision)
		FireBallCollision->OnComponentBeginOverlap.AddDynamic(this, &AMeteorite::OnFireBallOverlapBegin);
	
	if (ExplosionEffect)
		ExplosionEffect->OnSystemFinished.AddDynamic(this, &AMeteorite::OnExplosionComplete);
}

// Called every frame
void AMeteorite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);
	UpdateAttackRange();
	UpdateSize();
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

	if (OtherActor->IsA(AWitchBossActor::StaticClass()) || OtherActor->IsA(AMeteorite::StaticClass()))
		return;

	CanMove = false;
	// 自身メッシュとNiagaraを表示しない
	FireBallMesh->SetVisibility(false);
	FireEffect->Deactivate();
	if(AttackRangeDynamic)
		AttackRangeDynamic->SetScalarParameterValue(TEXT("Opacity"), 0);
	// 再判定しない
	FireBallCollision->SetCollisionProfileName(FName("NoCollision"));
	// 爆発エフェクト
	ShowExplosion();
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
	
	if (MaterialPoint != StartPoint)
	{
		AttackRangeDynamic->SetScalarParameterValue(TEXT("Percent"), 0);
		AttackRangeDynamic->SetScalarParameterValue(TEXT("Opacity"), 1);

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

		float Percent = FMath::Clamp((Total - Now) / Total, 0, 1);
		AttackRangeDynamic->SetScalarParameterValue(TEXT("Percent"), Percent);
	}
}

void AMeteorite::UpdateSize()
{
	if (CanMove)
	{
		float Total = FVector::Dist(StartPoint, EndPoint);
		float Now = FVector::Dist(GetActorLocation(), EndPoint);
		float Percent = FMath::Clamp((Total - Now) / Total, 0, 1);

		float Result = FMath::Lerp(MinSize, MaxSize, Percent);
		FireBallMesh->SetRelativeScale3D(FVector::OneVector * Result);
		FireBallCollision->SetRelativeScale3D(FVector::OneVector * Result);
	}
}

void AMeteorite::ShowExplosion()
{
	ExplosionEffect->Activate(true);
}

void AMeteorite::OnExplosionComplete(UParticleSystemComponent* PSystem)
{
	NotifyDisappear();
	Destroy();
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