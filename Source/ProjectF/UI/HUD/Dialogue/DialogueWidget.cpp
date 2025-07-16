// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidget.h"

#include "DialogueChoiceButton.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "ProjectF/Interaction/PFDialogueTypes.h"
#include "ProjectF/Manager/DialogueManager.h"
#include "ProjectF/Manager/PFGameInstance.h"

void UDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	int ChoiceButtonCnt = SelectButton_Vertical->GetChildrenCount();
	for (int i = 0; i < ChoiceButtonCnt; i++)
	{
		UDialogueChoiceButton* DialogueChoiceButton = Cast<UDialogueChoiceButton>(SelectButton_Vertical->GetChildAt(i));
		if (!DialogueChoiceButton) continue;
		
		DialogueChoiceButton->OnChoiceButtonClicked.AddDynamic(this, &UDialogueWidget::HandleChoiceButton);
	}
}

void UDialogueWidget::InitDialogue(FName DialogueID)
{
	if (!GetGameInstance()) return;

	UPFGameInstance* PFGameInstance = Cast<UPFGameInstance>(GetGameInstance());
	if (!PFGameInstance) return;
	
	UDialogueManager* DialogueManager = PFGameInstance->GetDialogueManager();
	if (!DialogueManager) return;
	
	FDialogueTableRow* DialogueTableRow = DialogueManager->GetDialogueTableRow(DialogueID);
	if (DialogueTableRow)
	{
		CurDialogueTableRow = DialogueTableRow;
		Name_Text->SetText(CurDialogueTableRow->DisplayName);
		Dialogue_Text->SetText(CurDialogueTableRow->Dialogue);
		
		int ChoicesCnt = SelectButton_Vertical->GetChildrenCount();
		
		switch (DialogueTableRow->DialogueNextType)
		{
		case EDialogueNextType::NextLine:
			SelectButton_Vertical->SetVisibility(ESlateVisibility::Collapsed);
			break;
		
		case EDialogueNextType::Choices:
			for (int i = 0; i < ChoicesCnt; ++i)
			{
				UDialogueChoiceButton* DialogueChoiceButton = Cast<UDialogueChoiceButton>(SelectButton_Vertical->GetChildAt(i));
				if (!DialogueChoiceButton) continue;
				
				if (CurDialogueTableRow->DialogueChoices.Num() > i)
				{
					DialogueChoiceButton->InitChoiceButton(CurDialogueTableRow->DialogueChoices[i]);
					DialogueChoiceButton->SetVisibility(ESlateVisibility::Visible);
				}
				else
				{
					DialogueChoiceButton->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
			break;

		case EDialogueNextType::EndDialogue:
			SelectButton_Vertical->SetVisibility(ESlateVisibility::Collapsed);
			break;
		}
	}
}


void UDialogueWidget::NextDialogue()
{
	if (!CurDialogueTableRow) return;

	if(CurDialogueTableRow->DialogueNextType == EDialogueNextType::EndDialogue)
	{
		OnDialogueEnd.Broadcast();
	}
	
	if (!GetGameInstance()) return;

	UPFGameInstance* PFGameInstance = Cast<UPFGameInstance>(GetGameInstance());
	if (!PFGameInstance) return;
	
	UDialogueManager* DialogueManager = PFGameInstance->GetDialogueManager();
	if (!DialogueManager) return;
	
	FDialogueTableRow* DialogueTableRow = DialogueManager->GetDialogueTableRow(CurDialogueTableRow->NextDialogueID);
	if (DialogueTableRow)
	{
		CurDialogueTableRow = DialogueTableRow;
		Name_Text->SetText(DialogueTableRow->DisplayName);
		Dialogue_Text->SetText(DialogueTableRow->Dialogue);
		
		int ChoicesCnt = SelectButton_Vertical->GetChildrenCount();
		
		switch (DialogueTableRow->DialogueNextType)
		{
		case EDialogueNextType::NextLine:
			SelectButton_Vertical->SetVisibility(ESlateVisibility::Collapsed);
			break;
		
		case EDialogueNextType::Choices:
			for (int i = 0; i < ChoicesCnt; ++i)
			{
				UDialogueChoiceButton* DialogueChoiceButton = Cast<UDialogueChoiceButton>(SelectButton_Vertical->GetChildAt(i));
				if (!DialogueChoiceButton) continue;
				
				if (CurDialogueTableRow->DialogueChoices.Num() > i)
				{
					DialogueChoiceButton->InitChoiceButton(CurDialogueTableRow->DialogueChoices[i]);
					DialogueChoiceButton->SetVisibility(ESlateVisibility::Visible);
				}
				else
				{
					DialogueChoiceButton->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
			SelectButton_Vertical->SetVisibility(ESlateVisibility::Visible);
			break;

		case EDialogueNextType::EndDialogue:
			SelectButton_Vertical->SetVisibility(ESlateVisibility::Collapsed);
			break;
		}
	}
}

void UDialogueWidget::HandleChoiceButton(UDialogueChoiceButton* Instigator, FDialogueChoiceInfo DialogueChoiceInfo)
{
	if (DialogueChoiceInfo.DialogueNextType == EDialogueNextType::EndDialogue)
	{
		OnDialogueEnd.Broadcast();
	}
	else if (DialogueChoiceInfo.DialogueNextType == EDialogueNextType::NextLine)
	{
		InitDialogue(DialogueChoiceInfo.NextDialogueID);		
	}
}
