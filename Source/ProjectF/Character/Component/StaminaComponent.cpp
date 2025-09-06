// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaComponent.h"

#include "Kismet/GameplayStatics.h"

UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentStamina = MaxStamina;
}

void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bAutoRecovery)
	{
		if (CanRegenCondition.IsBound())
		{
			if (CanRegenCondition.Execute())
			{
				UpdateStamina(AutoRecoveryValue * DeltaTime * UGameplayStatics::GetGlobalTimeDilation(GetWorld()));		
			}
		}
		else
		{
			UpdateStamina(AutoRecoveryValue * DeltaTime);
		}
	}
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
			
			if (CurrentStamina <= 0)
			{
				bAutoRecovery = false;
				GetWorld()->GetTimerManager().SetTimer(AutoRecoveryTimerHandle, this, &ThisClass::HandleRestartAutoRecovery, 3.0f, false);
			}
		}
	}
}

void UStaminaComponent::HandleRestartAutoRecovery()
{
	bAutoRecovery = true;
	UpdateStamina(MaxStamina);
}

void UStaminaComponent::InitStamina(float InMaxStamina)
{
	MaxStamina = InMaxStamina;
	CurrentStamina = MaxStamina;
}

void UStaminaComponent::AddStamina(float InValue)
{
	UpdateStamina(InValue);
}

