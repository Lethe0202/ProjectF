// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Grab.h"

#include "FrameTypes.h"
#include "InputModifiers.h"

void UAbility_Grab::AbilityEffect(int Index)
{
	Super::AbilityEffect(Index);

	if (!Owner.Get())
	{
		return;
	}
	
	if (Index == 0)
	{
		CacheOwnerLocation = Owner->GetActorLocation();
	}
	else if (Index == 1)
	{
		for (auto& EffectedActor : EffectedActors)
		{
			CacheTargetLocations.Emplace(EffectedActor->GetActorLocation());	
		}

		if (!EffectedActors.IsEmpty())
		{
			Owner->GetWorld()->GetTimerManager().SetTimer(GrabTimerHandle, this, &UAbility_Grab::OnGrabTimer, Owner->GetWorld()->GetDeltaSeconds(), true, -1);	
		}
	}
}

void UAbility_Grab::EndAbilityEffect(int Index)
{
	if (!Owner->GetWorld()->GetTimerManager().IsTimerActive(GrabTimerHandle))
	{
		Super::EndAbilityEffect(Index);
	}
}

bool UAbility_Grab::ActivateAbility()
{
	GrabTimerElapsed = 0.f;
	
	return Super::ActivateAbility();
}

void UAbility_Grab::OnGrabTimer()
{
	GrabTimerElapsed += Owner->GetWorld()->GetTimerManager().GetTimerElapsed(GrabTimerHandle);
		
	float LerpAlpha = GrabTimerElapsed / GrabDuration;
	float ClampLerpAlpha = FMath::Clamp(LerpAlpha, 0, 1);
	
	for (int i = 0; i < EffectedActors.Num(); ++i)
	{
		if (!CacheTargetLocations.IsValidIndex(i))
		{
			continue;
		}

		FVector Direction = (CacheOwnerLocation - CacheTargetLocations[i]).GetSafeNormal();
		
		FVector LerpLocation = FMath::Lerp(CacheTargetLocations[i], CacheOwnerLocation - Direction * 50.f, ClampLerpAlpha);
		EffectedActors[i]->SetActorLocation(LerpLocation, true);
	}
	
	if (ClampLerpAlpha >= 1)
	{
		Owner->GetWorld()->GetTimerManager().ClearTimer(GrabTimerHandle);
		EndAbilityEffect(1);
	}
}
