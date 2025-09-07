// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "JerryBeanAnimInstance.generated.h"

class AMeleeEnemy;

/**
 * 
 */
UCLASS()
class SWEETPROJECT_API UJerryBeanAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UJerryBeanAnimInstance();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="DeltaTime"></param>
	UFUNCTION(BlueprintCallable)
	void UpdateAnimation(float DeltaTime);

private:

	/// <summary>
	/// ワッフルのインスタンスを設定
	/// </summary>
	void SetJerryBeanInstance();

	/// <summary>
	/// ワッフル
	/// </summary>
	AMeleeEnemy* JerryBean;

	/// <summary>
	/// 生きているか
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsAlive;

	/// <summary>
	/// 攻撃するか
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsAttack;

	/// <summary>
	/// 移動するか
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsMove;
};
