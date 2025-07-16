// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "Ability_Wrath.generated.h"

/**
 * Montage 0 : 일반
 * Montage 1 : 홀드 성공
 * Montage 2 : 공중 공격
 */
UCLASS()
class PROJECTF_API UAbility_Wrath : public UAbilityBase
{
	GENERATED_BODY()
	
public:
	virtual bool ActivateAbility() override;
	virtual void HoldEnd(float HoldingTime) override;

protected:
	UFUNCTION()
	void OnCheckGroundTimer();
	
protected:
	FTimerHandle CheckGroundTimerHandle;
	
	UPROPERTY()
	TWeakObjectPtr<ACharacter> CacheCharacter;
};

