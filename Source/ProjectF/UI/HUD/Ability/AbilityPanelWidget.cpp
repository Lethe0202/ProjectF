// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityPanelWidget.h"

#include "ProjectF/Ability/AbilityBase.h"
#include "ProjectF/Ability/DataAsset/UISlotDataAsset.h"
#include "ProjectF/Character/Component/AbilityComponent.h"
#include "ProjectF/UI/HUD/Slot/CooldownSlotWidget.h"

void UAbilityPanelWidget::InitializeAbilitySlot(UAbilityComponent* AbilityComponent)
{
	for (int i = 0; i < AbilitySlots.Num(); ++i)
	{
		UAbilityBase* AbilityBase = AbilityComponent->GetAbilityByIndex(i);
		if (!AbilityBase) continue;
		
		UAbilityDataAsset* AbilityDataAsset = AbilityBase->GetAbilityDataAsset();
		if (!AbilityDataAsset) continue;
		
		UUISlotDataAsset* AbilityUIDataAsset= AbilityDataAsset->AbilityUIDataAsset;
		if (!AbilityUIDataAsset) continue;
		
		AbilitySlots[i]->SetIconImage(AbilityUIDataAsset->Icon);
	}
}

void UAbilityPanelWidget::HandleUpdateCooldown(int Index, float MaxCooldown, float CooldownElpased)
{
	if (!AbilitySlots.IsValidIndex(Index)) return;
		
	AbilitySlots[Index]->HandleUpdateCooldown(MaxCooldown, CooldownElpased);
}
