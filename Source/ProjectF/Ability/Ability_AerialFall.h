// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "Ability_AerialFall.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTF_API UAbility_AerialFall : public UAbilityBase
{
	GENERATED_BODY()

public:
	virtual bool ActivateAbility() override;
	virtual void AbilityEffect(int Index) override;
	virtual void EndAbilityEffect(int Index) override;
};
