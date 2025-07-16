// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "Ability_Sting.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTF_API UAbility_Sting : public UAbilityBase
{
	GENERATED_BODY()

public:
	virtual void AbilityEffect(int Index) override;
};
