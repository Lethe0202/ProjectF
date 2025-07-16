// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"

#include "ProgressHUDWidget.h"
#include "Ability/AbilityHoldProgressHUDWidget.h"
#include "Ability/AbilityPanelWidget.h"
#include "Ability/CooldownSlotWidget.h"
#include "Components/Overlay.h"
#include "ProjectF/Character/PFCharacterBase.h"
#include "ProjectF/Character/PFPlayerCharacter.h"
#include "ProjectF/Character/Component/AbilityComponent.h"
#include "ProjectF/Character/Component/HealthComponent.h"
#include "ProjectF/Character/Component/StaminaComponent.h"

void UPlayerHUDWidget::InitPlayerHUDWidget(APFPlayerCharacter* OwnerCharacter)
{
	if (!OwnerCharacter)
	{
		return;
	}
	
	if (UAbilityComponent* AbilityComponent = OwnerCharacter->GetAbilityComponent())
	{
		if (AbilityHoldProgress)
		{
			AbilityComponent->OnUpdateHoldProgress.AddDynamic(AbilityHoldProgress, &UAbilityHoldProgressHUDWidget::OnUpdateHoldProgress);	
		}
		
		if (AbilityPanel)
		{
			AbilityPanel->InitializeAbilitySlot(AbilityComponent);
			AbilityComponent->OnUpdateCooldown.AddDynamic(AbilityPanel, &UAbilityPanelWidget::HandleUpdateCooldown);
		}
	}

	if (UHealthComponent* HealthComponent = OwnerCharacter->GetHealthComponent())
	{
		if (HealthProgress)
		{
			HealthComponent->OnHealthStatusChanged.AddDynamic(HealthProgress, &UProgressHUDWidget::OnProgressChanged);
		}
	}
	
	if (UStaminaComponent* StaminaComponent = OwnerCharacter->GetStaminaComponent())
	{
		if (StaminaProgress)
		{
			StaminaComponent->OnStaminaChanged.AddDynamic(StaminaProgress, &UProgressHUDWidget::OnProgressChanged);
		}
	}
	
	if (APFPlayerCharacter* PFPlayerCharacter = Cast<APFPlayerCharacter>(OwnerCharacter))
	{
		PFPlayerCharacter->OnUpdatePotionCooldown.AddDynamic(PotionSlot, &UCooldownSlotWidget::HandleUpdateCooldown);
	}
}

void UPlayerHUDWidget::AddWidgetToSlot(FName SlotName, UUserWidget* Widget)
{
	if (RuntimeUISlotMap.Contains(SlotName))
	{
		RuntimeUISlotMap[SlotName]->AddChild(Widget);
	}
}
