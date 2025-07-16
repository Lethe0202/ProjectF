// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "Ability_RushMoveBase.generated.h"

/**
 *
 */
UCLASS()
class PROJECTF_API UAbility_RushMoveBase : public UAbilityBase
{
	GENERATED_BODY()
	
public:
	virtual bool ActivateAbility() override;
	virtual void AbilityEffect(int Index) override;
	virtual void EndAbilityEffect(int Index) override;

protected:
	UFUNCTION()
	void HandleDashTimer();
	
protected:
	UPROPERTY(EditAnywhere)
	float MovementSpeed = 0.f;
	
	UPROPERTY(EditAnywhere)
	float DashDuration = 0.f;
	
	FTimerHandle DashTimerHandle;

	float TimerStartTime;
	float OriginallyMovementSpeed;
};

