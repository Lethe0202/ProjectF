// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectF/Common/CombatTypes.h"
#include "CooldownSlotWidget.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class PROJECTF_API UCooldownSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
public:
	void InitializeAbilitySlot(UAbilityUIDataAsset* AbilityUIDataAsset);

	UFUNCTION()
	void HandleUpdateCooldown(float MaxCooldown, float CooldownElased);

	void SetIconImage(UTexture2D* InTexture);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> 	IconImage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CooldownImage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CooldownText;
	
protected:
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstanceDynamic;
};
