// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveManager.h"

#include "Kismet/GameplayStatics.h"
#include "ProjectF/Character/PFPlayerCharacter.h"
#include "ProjectF/Character/Component/HealthComponent.h"
#include "ProjectF/System/SaveGame/EnemySpawnSaveGame.h"
#include "ProjectF/System/SaveGame/PlayerSaveGame.h"

void USaveManager::SavePlayerInfo(APFPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter) return;
	
	USaveGame* PlayerSaveGameObject = UGameplayStatics::CreateSaveGameObject(UPlayerSaveGame::StaticClass());
	UPlayerSaveGame* PlayerSaveGame = Cast<UPlayerSaveGame>(PlayerSaveGameObject);
	
	PlayerSaveGame->LevelPath = PlayerCharacter->GetWorld()->GetOutermost()->GetName();
	PlayerSaveGame->PlayerSaveData.Transform = PlayerCharacter->GetTransform();
	PlayerSaveGame->PlayerSaveData.MaxHealthItemCnt = PlayerCharacter->GetMaxHealthItemCnt();
	
	UHealthComponent* HealthComponent = PlayerCharacter->GetHealthComponent();
	if (HealthComponent)
	{
		PlayerSaveGame->PlayerSaveData.CurrentHealth = HealthComponent->GetHealth();
		PlayerSaveGame->PlayerSaveData.MaxHealth = HealthComponent->GetMaxHealth();
	}
	
	UGameplayStatics::SaveGameToSlot(PlayerSaveGame, "PlayerData", 0);
}

void USaveManager::SaveEnemySpawn(const TSet<FString>& DeadEnemyID)
{
	USaveGame* EnemySpawnSaveGameObject = UGameplayStatics::CreateSaveGameObject(UEnemySpawnSaveGame::StaticClass());
	UEnemySpawnSaveGame* EnemySpawnSaveGame = Cast<UEnemySpawnSaveGame>(EnemySpawnSaveGameObject);
	EnemySpawnSaveGame->DeadEnemyID = DeadEnemyID;
	
	UGameplayStatics::SaveGameToSlot(EnemySpawnSaveGame, "EnemySpawn", 0);
}

void USaveManager::LoadPlayerInfo(APFPlayerCharacter* PlayerCharacter)
{
	USaveGame* PlayerSaveGameObject = UGameplayStatics::LoadGameFromSlot("PlayerData", 0);
	if (!PlayerSaveGameObject) return;
	
	UPlayerSaveGame* PlayerSaveGame = Cast<UPlayerSaveGame>(PlayerSaveGameObject);
	if (!PlayerSaveGame) return;
	
	PlayerCharacter->SetMaxHealthItemCnt(PlayerSaveGame->PlayerSaveData.MaxHealthItemCnt);
	PlayerCharacter->SetActorTransform(PlayerSaveGame->PlayerSaveData.Transform);
}

void USaveManager::LoadEnemySpawn(UWorld* World)
{
	FString PathName = World->GetOutermost()->GetName();
	UGameplayStatics::OpenLevel(World, *PathName);
}

void USaveManager::ClearGame()
{
	UGameplayStatics::DeleteGameInSlot("PlayerData", 0);
	UGameplayStatics::DeleteGameInSlot("EnemySpawn", 0);
}

UPlayerSaveGame* USaveManager::GetPlayerSaveGame()
{
	USaveGame* PlayerSaveGameObject = UGameplayStatics::LoadGameFromSlot("PlayerData", 0);
	if (!PlayerSaveGameObject) return nullptr;
	
	UPlayerSaveGame* PlayerSaveGame = Cast<UPlayerSaveGame>(PlayerSaveGameObject);
	return PlayerSaveGame; 
}

FString USaveManager::GetSaveLevelPath() const
{
	USaveGame* PlayerSaveGameObject = UGameplayStatics::LoadGameFromSlot("PlayerData", 0);
	if (!PlayerSaveGameObject) return FString();
	
	UPlayerSaveGame* PlayerSaveGame = Cast<UPlayerSaveGame>(PlayerSaveGameObject);
	if (!PlayerSaveGame) return FString();

	return PlayerSaveGame->LevelPath;
}

bool USaveManager::IsHaveSaveFile() const
{
	bool bSaved = UGameplayStatics::DoesSaveGameExist("PlayerData", 0);
	return bSaved;
}

TSet<FString> USaveManager::GetEnemySpawnSave()
{
	USaveGame* EnemySpawnSaveGameObject	= UGameplayStatics::LoadGameFromSlot("EnemySpawn", 0);

	if (!EnemySpawnSaveGameObject) return TSet<FString>();
	
	UEnemySpawnSaveGame* EnemySpawnSaveGame = Cast<UEnemySpawnSaveGame>(EnemySpawnSaveGameObject);
	if (!EnemySpawnSaveGame) return TSet<FString>();
	
	return EnemySpawnSaveGame->DeadEnemyID;
}
