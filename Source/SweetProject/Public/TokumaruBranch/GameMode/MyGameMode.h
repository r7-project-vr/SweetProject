// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

class APillar;

/**
 * 
 */
UCLASS()
class SWEETPROJECT_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMyGameMode();
	
	// 2025.08.29 ウー start
protected:

	virtual void BeginPlay() override;

private:

	/// <summary>
	/// 柱が壊れた時の処理
	/// </summary>
	UFUNCTION()
	void OnPillarDisappear();

	/// <summary>
	/// ゲームクリア
	/// </summary>
	void GameClear();

	/// <summary>
	/// クリアした後に行くマップ
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UWorld> LoadSuccLevel;

	/// <summary>
	/// 支えている柱
	/// </summary>
	TArray<AActor*> Pillars;

	/// <summary>
	/// 破壊された柱の数
	/// </summary>
	int DisappearedPillarNum;
	// 2025.08.29 ウー end

};
