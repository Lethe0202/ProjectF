// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSaveGame.generated.h"

/**
 * 
 */

USTRUCT()
struct FPlayerSaveData
{
	GENERATED_BODY()

	
public:
	UPROPERTY()
	FTransform Transform;
	
	UPROPERTY()
	float MaxHealth = 100.0f;
	
	UPROPERTY()
	float CurrentHealth = 100.0f;

	UPROPERTY()
    int MaxHealthItemCnt = 1;
};

UCLASS()
class PROJECTF_API UPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString LevelPath;

	UPROPERTY()
	FPlayerSaveData PlayerSaveData;
};
