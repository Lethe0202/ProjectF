// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PFGameInstance.generated.h"

class UCombatManager;
class UDialogueManager;
class UAnimManager;
/**
 * 
 */
UCLASS()
class PROJECTF_API UPFGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void Init();

	UAnimManager* GetAnimManager() { return AnimManager; }
	UDialogueManager* GetDialogueManager() { return DialogueManager; }
	UCombatManager* GetCombatManager() { return CombatManager; }
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAnimManager> AnimManagerClass;

	UPROPERTY()
	TObjectPtr<UAnimManager> AnimManager;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDialogueManager> DialogueManagerClass;

	UPROPERTY()
	TObjectPtr<UDialogueManager> DialogueManager;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCombatManager> CombatManagerClass;

	UPROPERTY()
	TObjectPtr<UCombatManager> CombatManager;
};
