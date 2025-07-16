// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ProjectF/Interaction/PFDialogueTypes.h"
#include "DialogueManager.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECTF_API UDialogueManager : public UObject
{
	GENERATED_BODY()

public:
	FDialogueTableRow* GetDialogueTableRow(FName DialogueID);

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> DialogueDataTable;
};
