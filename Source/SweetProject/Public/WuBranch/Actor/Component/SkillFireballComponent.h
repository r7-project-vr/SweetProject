// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillFireballComponent.generated.h"

class AMeteorite;
class ANavEnterSpawner;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWEETPROJECT_API USkillFireballComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillFireballComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// <summary>
	/// 火球を生成する
	/// </summary>
	/// <param name="Location">指定する位置、ないなら(0, 0, 0)</param>
	/// <param name="Rotator">指定する角度、ないなら(0, 0, 0)</param>
	/// <returns>角度</returns>
	AMeteorite* SpawnFireBall(FVector Location = FVector::ZeroVector, FRotator Rotator = FRotator::ZeroRotator);

	/// <summary>
	/// 火球を発射
	/// </summary>
	void Shoot();

	/// <summary>
	/// 攻撃範囲の半径をゲット
	/// </summary>
	/// <returns>攻撃範囲の半径</returns>
	float GetAttackRadius() const;

private:

	/// <summary>
	/// 火球のサンプル
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMeteorite> FireBallSample;

	/// <summary>
	/// 生成した火球
	/// </summary>
	AMeteorite* CurrentFireBall;

	/// <summary>
	/// 攻撃範囲の半径、1は1cm
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

#pragma region Lock
public:

	/// <summary>
	/// 火球の位置を固定するのを開始
	/// </summary>
	void StartLockFireBall();

	/// <summary>
	/// 火球の位置固定を解除
	/// </summary>
	void UnLockFireBall();

private:
	/// <summary>
	/// 火球の位置をを固定
	/// </summary>
	void LockFireBall();

	/// <summary>
	/// キャラクターのメッシュ
	/// </summary>
	USkeletalMeshComponent* Mesh;

	/// <summary>
	/// 火球を生成する位置のソケット名
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName FireballSocketName;

	/// <summary>
	/// 火球の位置を固定するフラグ
	/// </summary>
	bool IsLockedFireBallLocation;
#pragma endregion
};
