// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DonutLightStates.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EDonutLightStates : uint8
{
	// ドーナツのライトが点灯している状態
	DonutLightOn UMETA(DisplayName = "Donut Light On"),
	// ドーナツのライトが消灯している状態
	DonutLightOff UMETA(DisplayName = "Donut Light Off"),
};