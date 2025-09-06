// Fill out your copyright notice in the Description page of Project Settings.
	
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "EnemySpawnSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTF_API UEnemySpawnSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TSet<FString> DeadEnemyID;
};

