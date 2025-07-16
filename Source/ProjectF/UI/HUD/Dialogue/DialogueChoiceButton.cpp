// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueChoiceButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ProjectF/Interaction/PFDialogueTypes.h"

void UDialogueChoiceButton::InitChoiceButton(FDialogueChoiceInfo& DialogueChoice)
{
	CurrentDialogueChoice = DialogueChoice;
	Choice_Text->SetText(CurrentDialogueChoice.ChoiceDisplayText);

	Choice_Button->OnClicked.AddDynamic(this, &UDialogueChoiceButton::OnButtonClicked);
}

void UDialogueChoiceButton::OnButtonClicked()
{
	OnChoiceButtonClicked.Broadcast(this, CurrentDialogueChoice);
}
