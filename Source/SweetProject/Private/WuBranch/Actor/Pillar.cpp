// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/Pillar.h"
#include "Components/BoxComponent.h"

// Sets default values
APillar::APillar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	USceneComponent* myRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = myRoot;

	PillarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pillar Mesh"));
	PillarMesh->SetupAttachment(RootComponent);
	PillarMesh->OnComponentBeginOverlap.AddDynamic(this, &APillar::OnCollisionOverlapBegin);
	
	/*Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &APillar::OnCollisionOverlapBegin);*/
}

// Called when the game starts or when spawned
void APillar::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void APillar::Initialize()
{
	HP = MaxHP;
}

float APillar::GetHP() const
{
	return HP;
}

float APillar::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageToApply = FMath::Clamp(DamageToApply, 0.0f, HP);
	HP -= DamageToApply;

	// �j�󂳂ꂽ
	if (HP <= 0.0f)
	{
		// �G�t�F�N�g

		// �G�t�F�N�g�Ȃǂ��I����������
		Destroy();
	}
	else {
		// �G�t�F�N�g

	}
	return 0.0f;
}

void APillar::OnCollisionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �U���҂���U���͂�Ⴄ
	float Attack = 10;
	float DamageAmount = FMath::Clamp(Attack, 0.0f, HP); // ��Ƃ���10�̃_���[�W��^����
	HP -= DamageAmount;

	// �j�󂳂ꂽ
	if (HP <= 0.0f)
	{
		// �G�t�F�N�g

		// �G�t�F�N�g�Ȃǂ��I����������
		Destroy();
	}
	else {
		// �G�t�F�N�g

	}
}

// Called every frame
//void APillar::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

