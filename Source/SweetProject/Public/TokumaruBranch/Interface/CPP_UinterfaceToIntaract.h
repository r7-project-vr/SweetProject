// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CPP_UinterfaceToIntaract.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCPP_UinterfaceToIntaract : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SWEETPROJECT_API ICPP_UinterfaceToIntaract
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/// <summary>
    /// 対象をこちらから変更する。
    /// </summary>
    /// <param name="Interactor"></param>
	virtual void Interact(AActor* Interactor) = 0;

	/// <summary>
	/// 自分自身を変更
	/// </summary>
	virtual void BYInteract() = 0;
};
