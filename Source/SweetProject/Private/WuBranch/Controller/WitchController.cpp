// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Controller/WitchController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

AWitchController::AWitchController()
	: IsMoving(false)
{
}

void AWitchController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior)
	{
		RunBehaviorTree(AIBehavior);
		Blackboard = GetBlackboardComponent();

		InitBlackboard();
	}

	IsMoving = false;
}

void AWitchController::Tick(float DeltaTime)
{
	if (IsMoving)
		HandleMoving(DeltaTime);
}

bool AWitchController::IsArriveLocation(FVector TargetLocation)
{
	FVector Diff = TargetLocation - GetCharacter()->GetActorLocation();
	if (Diff.SizeSquared() <= FMath::Square(DisplacementTolerance))
	{
		return false;
	}

	return true;
}

void AWitchController::StartMovingToLocation(FVector TargetLocation, float AcceptanceRadius)
{
	// 移動中
	if (IsMoving)
		return;

	CurrentMoveTarget = TargetLocation;
	DisplacementTolerance = AcceptanceRadius;
	IsMoving = true;
}

bool AWitchController::GetIsMoving() const
{
	return IsMoving;
}

void AWitchController::InitBlackboard()
{
	if (Blackboard)
	{
		Blackboard->SetValueAsInt(TEXT("IsStart"), false);
	}
}

void AWitchController::HandleMoving(float DeltaTime)
{
	// 既に到着したか
	if (IsArriveLocation(CurrentMoveTarget))
	{
		IsMoving = false;
		NotifyMovementCompleted();
		return;
	}

	FVector CurrentLocation = GetCharacter()->GetActorLocation();
	FVector Direction = (CurrentMoveTarget - CurrentLocation).GetSafeNormal();
	GetCharacter()->SetActorLocation(CurrentLocation + Direction * MovementSpeed * DeltaTime);
}

void AWitchController::NotifyMovementCompleted()
{
	if (OnMoveCompletedNotify.IsBound())
		OnMoveCompletedNotify.Broadcast();
}
