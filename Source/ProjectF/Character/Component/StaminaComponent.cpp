// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaComponent.h"

UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentStamina = MaxStamina;
}

void UStaminaComponent::UpdateStamina(float InValue)
{
	if (InValue > 0)
	{
		if (CurrentStamina < MaxStamina)
		{
			float OldValue = CurrentStamina;
			CurrentStamina = FMath::Clamp(CurrentStamina + InValue, 0, MaxStamina);
			if (OldValue != CurrentStamina)
			{
				OnStaminaChanged.Broadcast(OldValue, CurrentStamina, MaxStamina, nullptr);	
			}
		}
	}
	else
	{
		if (CurrentStamina > 0)
		{
			float OldValue = CurrentStamina;
			CurrentStamina = FMath::Clamp(CurrentStamina + InValue, 0, MaxStamina);
			if (OldValue != CurrentStamina)
			{
				OnStaminaChanged.Broadcast(OldValue, CurrentStamina, MaxStamina, nullptr);
			}
		}
	}
}

void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateStamina(AutoRecoveryValue * DeltaTime);
}

void UStaminaComponent::AddStamina(float InValue)
{
	UpdateStamina(InValue);
}

