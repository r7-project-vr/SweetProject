// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WitchAnimInstance.generated.h"

class AWitchBossActor;

/**
 * 
 */
UCLASS()
class SWEETPROJECT_API UWitchAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UWitchAnimInstance();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="DeltaTime"></param>
	UFUNCTION(BlueprintCallable)
	void UpdateAnimation(float DeltaTime);

private:

	/// <summary>
	/// 魔女のインスタンスを設定
	/// </summary>
	void SetWitchInstance();

	/// <summary>
	/// 魔女
	/// </summary>
	AWitchBossActor* Witch;

	/// <summary>
	/// 攻撃しているか
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsAttack;


};
