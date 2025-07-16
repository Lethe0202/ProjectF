// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Sting.h"

#include "Components/CapsuleComponent.h"

void UAbility_Sting::AbilityEffect(int Index)
{
	Super::AbilityEffect(Index);

	if (Index == 0)
	{
		UCapsuleComponent* CapsuleComponent = Owner->GetComponentByClass<UCapsuleComponent>();
		CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}
	else if (Index == 1)
	{
		UCapsuleComponent* CapsuleComponent = Owner->GetComponentByClass<UCapsuleComponent>();
		CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}
}
