// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectF/Interaction/PFDialogueTypes.h"
#include "ProjectF/UI/Interface/HUDPanelSlotWidgetInterface.h"
#include "DialogueWidget.generated.h"

class UVerticalBox;
class UTextBlock;
class UDialogueChoiceButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnd);

/**
 * 
 */
UCLASS()
class PROJECTF_API UDialogueWidget : public UUserWidget, public IHUDPanelSlotWidgetInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	void InitDialogue(FName DialogueID);
	virtual FName GetHUDSlotName_Implementation() const override { return MainHUDSlotName; } 
	
protected:
	UFUNCTION(BlueprintCallable)
	void NextDialogue();

	UFUNCTION()
	void HandleChoiceButton(UDialogueChoiceButton* Instigator, FDialogueChoiceInfo DialogueChoiceInfo);
	
public:
	FOnDialogueEnd OnDialogueEnd;
	
protected:
	UPROPERTY(EditAnywhere)
	FName MainHUDSlotName;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Name_Text;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Dialogue_Text;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UVerticalBox> SelectButton_Vertical;
	
	FDialogueTableRow* CurDialogueTableRow;
};
