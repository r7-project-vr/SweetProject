// Fill out your copyright notice in the Description page of Project Settings.


#include "MurasameBranch/WitchBossActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
// 2025.09.01 ウー start
#include "TokumaruBranch/Pawn/CPP_TVRPawn.h"
#include "MurasameBranch/NavEnterSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "WuBranch/Actor/Component/SkillFireballComponent.h"
#include "WuBranch/Actor/Meteorite.h"
// 2025.09.01 ウー end
#include "Components/AudioComponent.h"

// Sets default values
AWitchBossActor::AWitchBossActor()
	: FireballSocketName("")
	, IsAttack(false)
	, IsStart(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//SetRootComponent(Root);

	// 2025.08.30 ウー start
	//WitchMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WitchMesh"));
	//WitchMesh->SetupAttachment(Root);
	// 2025.08.30 ウー end
	// 2025.09.01 ウー start
	SkillFireball = CreateDefaultSubobject<USkillFireballComponent>(TEXT("Skill Fireball"));
	// 2025.09.01 ウー end
	//モデルの向きに沿って調整する可能性ある


	//Audiocomponentの生成
	ChargeUpAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ChargeUpAudioComponent"));
	ChargeUpAudioComponent->SetupAttachment(RootComponent);
	ChargeUpAudioComponent->bAutoActivate = false;

}

// Called when the game starts or when spawned
void AWitchBossActor::BeginPlay()
{
	Super::BeginPlay();
	// 2025.09.02 ウー start
	IsStart = false;
	// 2025.09.02 ウー end
	// 2025.09.01 ウー start
	// プレレイヤーをゲット
	PlayerOverride = UGameplayStatics::GetActorOfClass(GetWorld(), ACPP_TVRPawn::StaticClass());

	AActor* Spawner = UGameplayStatics::GetActorOfClass(GetWorld(), ANavEnterSpawner::StaticClass());
	if (!Spawner)
	{
		Spawner = GetWorld()->SpawnActor(ANavEnterSpawner::StaticClass());
	}
	LocationSpawner = Cast<ANavEnterSpawner>(Spawner);
	// 2025.09.01 ウー end
}

// Called every frame
void AWitchBossActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AActor* Player = PlayerOverride.Get();
	if (!Player)
	{
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			Player = PC->GetPawn();
		}
	}
	if (!IsValid(Player)) return;

	FVector WitchLoc = GetActorLocation();
	FVector TargetLoc = Player->GetActorLocation();

	if (bYawOnly)
	{
		WitchLoc.Z = 0.f;
		TargetLoc.Z = 0.f;
	}

	const FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(WitchLoc, TargetLoc);
	const FRotator NewRot = bYawOnly ? FRotator(0.f, LookAt.Yaw, 0.f) : LookAt;
	SetActorRotation(NewRot);
}

// 2025.09.01 ウー start
void AWitchBossActor::Shoot()
{
	SkillFireball->Shoot();
}

bool AWitchBossActor::GetIsAttack() const
{
	return IsAttack;
}

void AWitchBossActor::StartAttack()
{
	IsAttack = true;

	// 効果音を再生
	if (ChargeUpSound && ChargeUpAudioComponent)
	{
		ChargeUpAudioComponent->SetSound(ChargeUpSound);
		ChargeUpAudioComponent->FadeIn(0.2f);//　円滑するよう
	}
}

void AWitchBossActor::CompleteAttack()
{
	IsAttack = false;
}

void AWitchBossActor::UseFireball(const FVector& TargetLocation, const FVector& StartLocation)
{
	//効果音関連
	if (ChargeUpAudioComponent && ChargeUpAudioComponent->IsPlaying())
	{
		ChargeUpAudioComponent->FadeOut(0.1f, 0.f);//　円滑するよう
	}
	if (ThrowSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ThrowSound, GetActorLocation());
	}


	// スタート位置
	FVector SocketLocation = StartLocation;

	USkeletalMeshComponent* MyMesh = GetMesh();
	if (MyMesh && MyMesh->DoesSocketExist(FireballSocketName))
		SocketLocation = MyMesh->GetSocketLocation(FireballSocketName);

	// 目標位置を探す
	FVector NearWitchLocation, NearPlayerLocation;
	if (LocationSpawner)
	{
		LocationSpawner->SetGroundRadius(SkillFireball->GetAttackRadius());
		LocationSpawner->GetStartAndEndLocation(NearWitchLocation, NearPlayerLocation);
	}

	// 火球を生成する
	AMeteorite* FireBall = SkillFireball->SpawnFireBall(SocketLocation);
	if (FireBall)
	{
		FireBall->SetTargetPoint(NearPlayerLocation);
		ACharacter* Player = Cast<ACharacter>(PlayerOverride);
		if(Player)
			FireBall->SetTarget(Player);
	}
}
// 2025.09.01 ウー end

// 2025.09.02 ウー start
bool AWitchBossActor::GetIsStart() const
{
	return IsStart;
}

void AWitchBossActor::StartMove()
{
	IsStart = true;
}
// 2025.09.02 ウー end
