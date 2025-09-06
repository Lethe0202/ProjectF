// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "Kismet/KismetMathLibrary.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::InitHealth(float InMaxHealth)
{
	MaxHealth = InMaxHealth;
	CurrentHealth = MaxHealth;
}

void UHealthComponent::ApplyHeal(float Amount)
{
	UpdateHealth(Amount, GetOwner());
}

void UHealthComponent::ApplyDamage(float Amount, AActor* Instigator)
{
	UpdateHealth(-Amount, Instigator);
}

void UHealthComponent::UpdateHealth(int Amount, AActor* Instigator)
{
	float OldValue = CurrentHealth;
	CurrentHealth = UKismetMathLibrary::FClamp(CurrentHealth + Amount, 0.f, MaxHealth);
	
	OnHealthStatusChanged.Broadcast(OldValue, CurrentHealth, MaxHealth, Instigator);
}

