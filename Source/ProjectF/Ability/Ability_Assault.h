// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "Ability_Assault.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTF_API UAbility_Assault : public UAbilityBase
{
	GENERATED_BODY()

public:
	virtual bool ActivateAbility() override;
	virtual void AbilityEffect(int Index) override;
	virtual void HoldEnd(float HoldingTime) override;
	virtual void EndAbility() override;

protected:
	bool bCycle = false;
	bool bLastStrongAttack = false;
};
