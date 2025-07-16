// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AbilityBase.generated.h"

class APFCharacterBase;
class UAbilityDataAsset;


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECTF_API UAbilityBase : public UObject
{
	GENERATED_BODY()

public:
	UAbilityBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual bool ActivateAbility();
	virtual void AbilityEffect(int Index);
	virtual void EndAbilityEffect(int Index);
	virtual void HoldEnd(float HoldingTime);
	virtual void EndAbility();

	virtual bool GetHoldActivate() const { return bHoldActivate; }

	UAbilityDataAsset* GetAbilityDataAsset() const { return AbilityDataAsset; }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilityDataAsset> AbilityDataAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMultiHit = false;
	
	UPROPERTY()
	TWeakObjectPtr<AActor> Owner;
	
	UPROPERTY()
	TArray<AActor*> EffectedActors;
	
	bool bLaunch = false;
	bool bHoldActivate = false;
};
