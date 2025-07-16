// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectF/Common/CombatTypes.h"
#include "UObject/Object.h"
#include "AnimManager.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECTF_API UAnimManager : public UObject
{
	GENERATED_BODY()

public:
	void InitAnimManager();
	UAnimMontage* GetWeaponEquip(FName ID) const;
	UAnimMontage* GetWeaponUnEquip(FName ID) const;
	UAnimMontage* GetHitDirection(FName ID, ECharacterDirection Direction) const;
	UAnimMontage* GetRollDirection(FName ID, ECharacterDirection Direction) const;
	UAnimMontage* GetRollRandomDirection(FName ID) const;
	UAnimMontage* GetHitInAir(FName ID) const;
	UAnimMontage* GetTakeDownRecover(FName ID) const;
	UAnimMontage* GetStun(FName ID) const;
	
	
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> CombatAnimTable;
	
	TMap<FName, FCombatAnimTableRow*> CombatAnimMap;

	UPROPERTY()
	UAnimManager* AnimManager;
};
