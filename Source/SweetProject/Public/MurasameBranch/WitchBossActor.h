// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WitchBossActor.generated.h"

class USoundBase;
class UAudioComponent;

UCLASS()
class SWEETPROJECT_API AWitchBossActor : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWitchBossActor();

protected:
	// 2025.08.30 ウー start
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//USceneComponent* Root;
	
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//USkeletalMeshComponent* WitchMesh;
	// 2025.08.30 ウー end

	//魔女顔向きZ軸のみか
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Witch|Facing")
	bool bYawOnly = false;

	//プレイヤActor指定
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Witch|Facing")
	TWeakObjectPtr<AActor> PlayerOverride;

	// Called when the game starts or when spawned
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;


#pragma region Audio
protected:
	/// <summary>
	/// 投げるまでの溜めモーション効果音
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Witch|Audio")
	USoundBase* ChargeUpSound;

	/// <summary>
	/// 投げる瞬間効果音
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Witch|Audio")
	USoundBase* ThrowSound;

	/// <summary>
	/// 再生用コンポーネント
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Witch|Audio")
	UAudioComponent* ChargeUpAudioComponent;
#pragma endregion 

public:	
	UFUNCTION(BlueprintCallable, Category = "Witch|Facing")
	void SetYawOnly(bool bInYawOnly) { bYawOnly = bInYawOnly; }

	// 2025.09.01 ウー start
#pragma region Fireball

public:

	/// <summary>
	/// 火球を発射
	/// </summary>
	void Shoot();

	/// <summary>
	/// 攻撃しているか
	/// </summary>
	bool GetIsAttack() const;

	/// <summary>
	/// 攻撃開始
	/// </summary>
	void StartAttack();

	/// <summary>
	/// 攻撃完了
	/// </summary>
	void CompleteAttack();

	/// <summary>
	/// 火球を使う
	/// </summary>
	/// <param name="TargetLocation">目標位置</param>
	/// <param name="TargetLocation">開始位置、ないなら(0, 0, 0)</param>
	void UseFireball(const FVector& TargetLocation = FVector::ZeroVector, const FVector& StartLocation = FVector::ZeroVector);

private:
	/// <summary>
	/// スキル：火球
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USkillFireballComponent* SkillFireball;

	/// <summary>
	/// 位置生成するもの
	/// </summary>
	class ANavEnterSpawner* LocationSpawner;

	/// <summary>
	/// 火球のスタート位置のソケット名
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Witch", meta = (AllowPrivateAccess = "true"))
	FName FireballSocketName;

	/// <summary>
	/// 攻撃のフラグ
	/// </summary>
	bool IsAttack;
#pragma endregion
	// 2025.09.01 ウー end

	// 2025.09.02 ウー start
public:

	/// <summary>
	/// 動き始めているか
	/// </summary>
	/// <returns>true: はい, false: いいえ</returns>
	bool GetIsStart() const;

	/// <summary>
	/// 動き始める
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void StartMove();

private:

	/// <summary>
	/// 動き始めているフラグ
	/// </summary>
	bool IsStart;
	// 2025.09.02 ウー end
};
