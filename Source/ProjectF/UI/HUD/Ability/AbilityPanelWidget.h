// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilityPanelWidget.generated.h"

class UAbilityComponent;
class UCooldownSlotWidget;
/**
 * 
 */
UCLASS()
class PROJECTF_API UAbilityPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void InitializeAbilitySlot(UAbilityComponent* AbilityComponent);
	
	UFUNCTION()
	void HandleUpdateCooldown(int Index, float MaxCooldown, float CooldownElpased);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UCooldownSlotWidget>> AbilitySlots;
};
