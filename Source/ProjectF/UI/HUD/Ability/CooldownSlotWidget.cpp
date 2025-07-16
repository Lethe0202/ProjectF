// Fill out your copyright notice in the Description page of Project Settings.


#include "CooldownSlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCooldownSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	MaterialInstanceDynamic = CooldownImage->GetDynamicMaterial();

	CooldownImage->SetVisibility(ESlateVisibility::Collapsed);
	CooldownText->SetVisibility(ESlateVisibility::Collapsed);
}

void UCooldownSlotWidget::InitializeAbilitySlot(UAbilityUIDataAsset* AbilityUIDataAsset)
{
	if (AbilityUIDataAsset->Icon)
	{
		IconImage->SetBrushFromTexture(AbilityUIDataAsset->Icon);
	}
}

void UCooldownSlotWidget::HandleUpdateCooldown(float MaxCooldown, float CooldownElased)
{
	if (MaterialInstanceDynamic)
	{
		if (MaxCooldown > CooldownElased)
		{
			MaterialInstanceDynamic->SetScalarParameterValue("Cooldown", CooldownElased / MaxCooldown);
			CooldownImage->SetVisibility(ESlateVisibility::Visible);
			
			FString CooldownString = FString::FromInt(MaxCooldown - CooldownElased);
			CooldownText->SetText(FText::FromString(CooldownString));
			CooldownText->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			CooldownImage->SetVisibility(ESlateVisibility::Collapsed);
			CooldownText->SetVisibility(ESlateVisibility::Collapsed);
		}
		
	}
}

void UCooldownSlotWidget::SetIconImage(UTexture2D* InTexture)
{
	if (IconImage)
	{
		IconImage->SetBrushFromTexture(InTexture);
	}
}
