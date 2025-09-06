// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectF/System/SaveGame/PlayerSaveGame.h"
#include "UObject/Object.h"
#include "SaveManager.generated.h"

class APFPlayerCharacter;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECTF_API USaveManager : public UObject
{
	GENERATED_BODY()

public:
	void SavePlayerInfo(APFPlayerCharacter* PlayerCharacter);
	void SaveEnemySpawn(const TSet<FString>& DeadEnemyID);

	void LoadPlayerInfo(APFPlayerCharacter* PlayerCharacter);
	void LoadEnemySpawn(UWorld* World);

	void ClearGame();
	
	UPlayerSaveGame* GetPlayerSaveGame();
	FString GetSaveLevelPath() const;
	
	UFUNCTION(BlueprintCallable)
	bool IsHaveSaveFile() const;
	
	TSet<FString> GetEnemySpawnSave();
};



