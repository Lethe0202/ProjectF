// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueManager.h"

FDialogueTableRow* UDialogueManager::GetDialogueTableRow(FName DialogueID)
{
	if (!DialogueDataTable) return nullptr;
	
	FDialogueTableRow* DialogueTableRow = DialogueDataTable->FindRow<FDialogueTableRow>(DialogueID, "");

	return DialogueTableRow;
}
