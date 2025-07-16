// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "Ability_Grab.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTF_API UAbility_Grab : public UAbilityBase
{
	GENERATED_BODY()

public:
	virtual bool ActivateAbility() override;
	virtual void AbilityEffect(int Index) override;
	virtual void EndAbilityEffect(int Index) override;
	
protected:
	UFUNCTION()
	void OnGrabTimer();
	

protected:
	UPROPERTY(EditAnywhere)
	float GrabDuration;

	float GrabTimerElapsed;
	
	FTimerHandle GrabTimerHandle;

	FVector CacheOwnerLocation;
	TArray<FVector> CacheTargetLocations;
};
