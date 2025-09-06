// Fill out your copyright notice in the Description page of Project Settings.


#include "MurasameBranch/MeleeEnemy.h"
#include "MurasameBranch/EnemyAIController.h"
#include "AIController.h"

AMeleeEnemy::AMeleeEnemy()
{
	//敵をAI持たせ
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//指定AIController
	AIControllerClass = AEnemyAIController::StaticClass();

}

float AMeleeEnemy::GetDesiredAttackRange_Implementation() const
{
	//近接攻撃範囲です
	return Stats ? Stats->MeleeRange : 150.f;
}


