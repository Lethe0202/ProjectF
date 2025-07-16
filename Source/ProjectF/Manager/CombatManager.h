// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CombatManager.generated.h"

class UAbilityDataAsset;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECTF_API UCombatManager : public UObject
{
	GENERATED_BODY()
	
public:
	void StrongAttackVisualEffect(AActor* Instigator);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UParticleSystem> AIStrongAttackParticleSystem;
};
