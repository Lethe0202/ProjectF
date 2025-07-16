// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectF/Interaction/PFDialogueTypes.h"
#include "DialogueChoiceButton.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChoiceButtonClicked, UDialogueChoiceButton*, Instigator, FDialogueChoiceInfo, DialogueChoiceInfo);

/**
 * 
 */
UCLASS()
class PROJECTF_API UDialogueChoiceButton : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitChoiceButton(FDialogueChoiceInfo& DialogueChoice);

	UButton* GetButton() const { return Choice_Button; }

protected:
	UFUNCTION()
	void OnButtonClicked();
	
public:
	FOnChoiceButtonClicked OnChoiceButtonClicked;
	
protected:
	FDialogueChoiceInfo CurrentDialogueChoice;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Choice_Button;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Choice_Text;
};
