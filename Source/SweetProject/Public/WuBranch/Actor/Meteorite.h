// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Meteorite.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UParticleSystemComponent;

UCLASS()
class SWEETPROJECT_API AMeteorite : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeteorite();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// 目標の位置を設定
	/// </summary>
	/// <param name="Point">目標の位置</param>
	void SetTargetPoint(FVector Point);

	/// <summary>
	/// 発射
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void Shoot();

	/// <summary>
	/// 消失の通知
	/// </summary>
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDisappearDelegate);
	FDisappearDelegate OnDisappearNotify;

	/// <summary>
	/// 目標を設定
	/// </summary>
	/// <param name="Target">目標</param>
	void SetTarget(TWeakObjectPtr<ACharacter> Target);

	/// <summary>
	/// 攻撃範囲の半径を設定
	/// </summary>
	/// <param name="Radius">半径</param>
	void SetAttackRangeRadius(float Radius);

private:

	/// <summary>
	/// ぶつかる処理
	/// </summary>
	UFUNCTION()
	void OnFireBallOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="DeltaTime"></param>
	void Move(float DeltaTime);

	/// <summary>
	/// 地面に攻撃範囲を作る
	/// </summary>
	void MakeAttackRange();

	/// <summary>
	/// 攻撃範囲を表示するの座標をゲット
	/// </summary>
	/// <param name="OHitNormal">目標地点の法線</param>
	/// <returns>座標</returns>
	FVector GetAttackRangeLocation(FVector& OHitNormal);

	/// <summary>
	/// 攻撃範囲のエフェクトの更新
	/// </summary>
	void UpdateAttackRange();

	/// <summary>
	/// サイズの更新
	/// </summary>
	void UpdateSize();

	/// <summary>
	/// 爆発エフェクトを表示
	/// </summary>
	void ShowExplosion();

	/// <summary>
	/// 爆発が終わった
	/// </summary>
	/// <param name="PSystem"></param>
	UFUNCTION()
	void OnExplosionComplete(UParticleSystemComponent* PSystem);

	/// <summary>
	/// ダメージ計算
	/// </summary>
	void HandleDamage();

	/// <summary>
	/// 消滅の通知
	/// </summary>
	void NotifyDisappear();

	/// <summary>
	/// 火球のメッシュ
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FireBallMesh;

	/// <summary>
	/// 火球のコリジョン
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USphereComponent* FireBallCollision;

	/// <summary>
	/// 火のエフェクト
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> FireEffect;

	/// <summary>
	/// 爆発のエフェクト
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ExplosionEffect;

	/// <summary>
	/// 目標
	/// </summary>
	TWeakObjectPtr<ACharacter> CurrentTarget;

	/// <summary>
	/// 攻撃範囲のマテリアル
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* AttackRange;

	/// <summary>
	/// 実際に使う攻撃範囲のマテリアル
	/// </summary>
	UMaterialInstanceDynamic* AttackRangeDynamic;

	/// <summary>
	/// 開始位置
	/// </summary>
	FVector StartPoint;

	/// <summary>
	/// 最終位置
	/// </summary>
	FVector EndPoint;

	/// <summary>
	/// 移動できるか
	/// </summary>
	bool CanMove;

	/// <summary>
	/// 移動スピード
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Speed;

	/// <summary>
	/// 攻撃範囲の半径、1は1cm
	/// </summary>
	float AttackRangeRadius;

	/// <summary>
	/// 最小サイズ
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MinSize;

	/// <summary>
	/// 最大サイズ
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MaxSize;
};
