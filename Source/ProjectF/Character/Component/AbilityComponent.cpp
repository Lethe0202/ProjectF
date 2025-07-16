// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"

#include "ProjectF/Ability/AbilityBase.h"
#include "ProjectF/Character/PFCharacterBase.h"

class UEnhancedInputLocalPlayerSubsystem;

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner())
	{
		return;
	}
	
	OwnerCharacter = Cast<APFCharacterBase>(GetOwner());
	
	for (auto& AbilityClass : AbilitiesClass)
	{
		UAbilityBase* AbilityBase = NewObject<UAbilityBase>(GetOwner(), AbilityClass);
		RegisterAbility(AbilityBase);
	}
	
	AbilityCooldownInfo.Init(FAbilityCooldownInfo(), AbilitiesClass.Num());
}

// Called every frame
void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAbilityComponent::OnHoldingAbility(int Index, float HoldingTime)
{
	if (CurrentActivateAbility.IsValid())
	{
		if(!CurrentActivateAbility->GetHoldActivate()) return;
			
		UAbilityDataAsset* AbilityDataAsset= CurrentActivateAbility->GetAbilityDataAsset();
		if (IsValid(AbilityDataAsset))
		{
			OnUpdateHoldProgress.Broadcast(AbilityDataAsset->MaxHoldTime, HoldingTime, AbilityDataAsset->MaxPerfectTiming, AbilityDataAsset->MinPerfectTiming);	
		}
	}
}

void UAbilityComponent::OnReleasedAbility(int Index, float HoldingTime)
{
	if (CurrentActivateAbility.IsValid())
	{
		CurrentActivateAbility->HoldEnd(HoldingTime);
		OnUpdateHoldProgress.Broadcast(0.f, 0.f, 0.f, 0.f);
	}
}

void UAbilityComponent::Input_ActivateAbility(TSubclassOf<UAbilityBase> AbilityClass)
{
	ActivateAbility(AbilityClass);
}

bool UAbilityComponent::ActivateAbility(int Index)
{
	if (Abilities.IsValidIndex(Index))
	{
		if (!CanActivateAbility(Index))
		{
			return false;
		}
		
		UAbilityDataAsset* DataAsset = Abilities[Index]->GetAbilityDataAsset();
		if (!IsValid(DataAsset))
		{
			return false;	
		}
		
		if (!Abilities[Index]->ActivateAbility())
		{
			return false;
		}
		
		CurrentActivateAbility = Abilities[Index];
		ApplyCooldown(Index);
		
		return true;	
	}

	return false;
}

bool UAbilityComponent::ActivateAbility(TSubclassOf<UAbilityBase> AbilityClass)
{
	for (int i = 0; i < Abilities.Num(); ++i)
	{
		UAbilityBase* Ability = Abilities[i];
		if (Ability->GetClass()->IsChildOf(AbilityClass))
		{
			if (!CanActivateAbility(i))
			{
				return false;
			}
			
			const UAbilityDataAsset* DataAsset = Ability->GetAbilityDataAsset();
			if (!IsValid(DataAsset))
			{
				return false;
			}
			
			if (!Ability->ActivateAbility())
			{
				return false;
			}
			
			CurrentActivateAbility = Ability;
			ApplyCooldown(Abilities.IndexOfByKey(Ability));
			
			return true;
		}
	}

	return false;
}

bool UAbilityComponent::CanActivateAbility(int Index)
{
	if (!AbilityCooldownInfo.IsValidIndex(Index)) return false;
	if (GetWorld()->GetTimerManager().IsTimerActive(AbilityCooldownInfo[Index].TimerHandle)) return false;
	
	return true;
}

void UAbilityComponent::ApplyCooldown(int Index)
{
	if (!CurrentActivateAbility.IsValid()) return;
	if (!AbilityCooldownInfo.IsValidIndex(Index)) return;
	
	const UAbilityDataAsset* AbilityDataAsset = CurrentActivateAbility->GetAbilityDataAsset();
	if (!IsValid(AbilityDataAsset)) return;
	
	AbilityCooldownInfo[Index].Cooldown = AbilityDataAsset->Cooldown;
	AbilityCooldownInfo[Index].CooldownStartTime = GetWorld()->GetTimeSeconds();
	
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UAbilityComponent::HandleCooldownUpdate, Index);
	GetWorld()->GetTimerManager().SetTimer(AbilityCooldownInfo[Index].TimerHandle, TimerDelegate, GetWorld()->GetDeltaSeconds(), true);
}

void UAbilityComponent::HandleCooldownUpdate(int Index)
{
	FAbilityCooldownInfo& CooldownInfo = AbilityCooldownInfo[Index];
	float CooldownElapsed = GetWorld()->GetTimeSeconds() - CooldownInfo.CooldownStartTime;
	
	OnUpdateCooldown.Broadcast(Index, CooldownInfo.Cooldown, CooldownElapsed);

	if (CooldownElapsed >= CooldownInfo.Cooldown)
	{
		GetWorld()->GetTimerManager().ClearTimer(AbilityCooldownInfo[Index].TimerHandle);
	}
}

void UAbilityComponent::AbilityEffect(TSubclassOf<UAbilityBase> AbilityClass, int Index)
{
	for (auto& Ability : Abilities)
	{
		if (Ability->GetClass()->IsChildOf(AbilityClass))
		{
			Ability->AbilityEffect(Index);
		}
	}
}

void UAbilityComponent::EndAbilityEffect(TSubclassOf<UAbilityBase> AbilityClass, int Index)
{
	for (auto& Ability : Abilities)
	{
		if (Ability->GetClass()->IsChildOf(AbilityClass))
		{
			Ability->EndAbilityEffect(Index);
		}
	}
}

void UAbilityComponent::RegisterAbility(UAbilityBase* Ability)
{
	Abilities.AddUnique(Ability);
}

void UAbilityComponent::UnregisterAbility(UAbilityBase* Ability)
{
	Abilities.Remove(Ability);
}

void UAbilityComponent::EndAbility()
{
	if (CurrentActivateAbility.IsValid())
	{
		CurrentActivateAbility->EndAbility();
	}
	
	CurrentActivateAbility = nullptr;
}

void UAbilityComponent::StopAbility()
{
	if (CurrentActivateAbility.IsValid())
    {
    	CurrentActivateAbility->EndAbility();
		OnUpdateHoldProgress.Broadcast(0.f, 0.f, 0.f, 0.f);
    }
	
	CurrentActivateAbility = nullptr;
}

void UAbilityComponent::ClearAbility()
{
	Abilities.Empty();
}

