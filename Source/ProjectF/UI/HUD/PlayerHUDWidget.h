// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UCooldownSlotWidget;
class UOverlay;
class APFPlayerCharacter;
class UAbilityHoldProgressHUDWidget;
class UProgressHUDWidget;
class UAbilityPanelWidget;

/**
 * 
 */
UCLASS()
class PROJECTF_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitPlayerHUDWidget(APFPlayerCharacter* OwnerCharacter);

	void AddWidgetToSlot(FName SlotName, UUserWidget* Widget);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAbilityHoldProgressHUDWidget> AbilityHoldProgress;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressHUDWidget> HealthProgress;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressHUDWidget> StaminaProgress;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAbilityPanelWidget> AbilityPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCooldownSlotWidget> PotionSlot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, UOverlay*> RuntimeUISlotMap;
};
