// Fill out your copyright notice in the Description page of Project Settings.


#include "PFGameInstance.h"

#include "AnimManager.h"
#include "CombatManager.h"
#include "DialogueManager.h"

void UPFGameInstance::Init()
{
	Super::Init();

	if (AnimManagerClass)
	{
		AnimManager = NewObject<UAnimManager>(this, AnimManagerClass);
		if (AnimManager)
		{
			AnimManager->InitAnimManager();
		}
	}

	if (DialogueManagerClass)
	{
		DialogueManager = NewObject<UDialogueManager>(this, DialogueManagerClass);
	}

	if (CombatManagerClass)
	{
		CombatManager = NewObject<UCombatManager>(this, CombatManagerClass);
	}
}
