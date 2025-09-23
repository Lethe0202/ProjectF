// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PFAssetManager.h"
#include "Engine/GameInstance.h"
#include "PFGameInstance.generated.h"

class USoundManager;
class APFPlayerCharacter;
class USaveManager;
class UEnemyManager;
class UResourceManager;
class UCombatManager;
class UDialogueManager;
class UAnimManager;
class UProgressHUDWidget;

/**
 * 
 */
UCLASS()
class PROJECTF_API UPFGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void Init();
	
	virtual void Shutdown() override;

	void ReadyPlayerCharacter(APFPlayerCharacter* PlayerCharacter);
	
	UFUNCTION(BlueprintCallable)
	void ChangeLevel( const TSoftObjectPtr<UWorld> NewLevel);

	UFUNCTION(BlueprintCallable)
	void SaveGame(APFPlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintCallable)
	void LoadGame();

	UFUNCTION(BlueprintCallable)
	void ClearGame();

	UFUNCTION(BlueprintCallable)
	void PlayBGM(UWorld* World);
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> LoadingScreenWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> LoadingScreenWidget;
	
	FTSTicker::FDelegateHandle TickHandle;
	
	bool bPreOpenLevel = false;
	FString TargetLevelPath;
	
	UAnimManager* GetAnimManager() { return AnimManager; }
	UDialogueManager* GetDialogueManager() { return DialogueManager; }
	UCombatManager* GetCombatManager() { return CombatManager; }
	UResourceManager* GetResourceManager() { return ResourceManager; }
	UEnemyManager* GetEnemyManager() { return EnemyManager; }

	UFUNCTION(BlueprintCallable)
	USaveManager* GetSaveManager() { return SaveManager; }
	
	UFUNCTION(BlueprintCallable)
	UPFAssetManager* GetPFAssetManager() { return PFAssetManager;}
	
protected:
	void HandlePostWorldInitialization(UWorld* World, const UWorld::InitializationValues IVS);

	FString GetLevelCleanPath(FString Path) const;
	
	bool bMonsterSpawned = false;
	
	UFUNCTION()
	void HandleStartLoading();

	void HandleLoadEnd();

	FString CurrentLoadingLevelPath;

private:
	UPROPERTY(EditDefaultsOnly, Category="Manager")
	TSubclassOf<UAnimManager> AnimManagerClass;

	UPROPERTY()
	TObjectPtr<UAnimManager> AnimManager;

	UPROPERTY(EditDefaultsOnly, Category="Manager")
	TSubclassOf<UDialogueManager> DialogueManagerClass;

	UPROPERTY()
	TObjectPtr<UDialogueManager> DialogueManager;
	
	UPROPERTY(EditDefaultsOnly, Category="Manager")
	TSubclassOf<UCombatManager> CombatManagerClass;

	UPROPERTY()
	TObjectPtr<UCombatManager> CombatManager;

	UPROPERTY(EditDefaultsOnly, Category="Manager")
	TSubclassOf<UResourceManager> ResourceManagerClass;

	UPROPERTY()
	TObjectPtr<UResourceManager> ResourceManager;
	
	UPROPERTY(EditDefaultsOnly, Category="Manager")
	TSubclassOf<UEnemyManager> EnemyManagerClass;

	UPROPERTY()
	TObjectPtr<UEnemyManager> EnemyManager;

	UPROPERTY(EditDefaultsOnly, Category="Manager")
	TSubclassOf<USaveManager> SaveManagerClass;
	
	UPROPERTY()
	TObjectPtr<USaveManager> SaveManager;

	UPROPERTY(EditDefaultsOnly, Category="Manager")
	TSubclassOf<USoundManager> SoundManagerClass;
	
	UPROPERTY()
	TObjectPtr<USoundManager> SoundManager;

	UPROPERTY(EditDefaultsOnly, Category="Manager")
	TSubclassOf<UPFAssetManager> PFAssetManagerClass;

	UPROPERTY()
	TObjectPtr<UPFAssetManager> PFAssetManager;
	
	UPROPERTY()
	TObjectPtr<UWorld> CacheWorld = nullptr;
	
	UPROPERTY(EditAnywhere)
	TMap<FPrimaryAssetId, FString> LevelNameMap;

	UPROPERTY(EditAnywhere)
	bool bDebugMode = false;
};

