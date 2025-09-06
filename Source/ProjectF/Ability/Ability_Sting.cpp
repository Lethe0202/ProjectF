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
		
		TempStartLocation = Owner->GetActorLocation();
		TempEndLocation = TempStartLocation + Owner->GetActorForwardVector() * MovementLength;
		
		Owner->GetWorld()->GetTimerManager().SetTimer(StingTimerHandle, this, &ThisClass::HandleStingMovement, Owner->GetWorld()->GetDeltaSeconds(), true, -1.f);
	}
	else if (Index == 1)
	{
		//UCapsuleComponent* CapsuleComponent = Owner->GetComponentByClass<UCapsuleComponent>();
		//CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}
}

void UAbility_Sting::HandleStingMovement()
{
	TempElapsedTime += Owner->GetWorld()->GetTimerManager().GetTimerElapsed(StingTimerHandle);

	float Alpha = TempElapsedTime / StingTime;
		
	FVector NewLocation = FMath::Lerp(TempStartLocation, TempEndLocation, Alpha);
	Owner->SetActorLocation(NewLocation);
	
	if (Alpha >= 1.f)
	{
		UCapsuleComponent* CapsuleComponent = Owner->GetComponentByClass<UCapsuleComponent>();
		CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

		TempElapsedTime = 0.0f;
		Owner->GetWorld()->GetTimerManager().ClearTimer(StingTimerHandle);
	}
}
