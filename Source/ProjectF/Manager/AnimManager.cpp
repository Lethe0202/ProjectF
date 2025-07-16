// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimManager.h"
#include "ProjectF/Common/CombatTypes.h"

void UAnimManager::InitAnimManager()
{
	AnimManager = this;
}

UAnimMontage* UAnimManager::GetWeaponEquip(FName ID) const
{
	FCombatAnimTableRow* CombatAnimRow = CombatAnimTable->FindRow<FCombatAnimTableRow>(ID, "");
	if (CombatAnimRow)
	{
		if (CombatAnimRow->WeaponEquip)
		{
			return CombatAnimRow->WeaponEquip;
		}
	}
	
	return nullptr;
}

UAnimMontage* UAnimManager::GetWeaponUnEquip(FName ID) const
{
	FCombatAnimTableRow* CombatAnimRow = CombatAnimTable->FindRow<FCombatAnimTableRow>(ID, "");
	if (CombatAnimRow)
	{
		if (CombatAnimRow->WeaponUnEquip)
		{
			return CombatAnimRow->WeaponUnEquip;
		}
	}
	
	return nullptr;
}

UAnimMontage* UAnimManager::GetHitDirection(FName ID, ECharacterDirection Direction) const
{
	FCombatAnimTableRow* CombatAnimRow = CombatAnimTable->FindRow<FCombatAnimTableRow>(ID, "");
	if (CombatAnimRow)
	{
		if (CombatAnimRow->HitDirectionAnim.Contains(Direction))
		{
			return CombatAnimRow->HitDirectionAnim[Direction];	
		}
	}
	
	return nullptr;
}

UAnimMontage* UAnimManager::GetRollDirection(FName ID, ECharacterDirection Direction) const
{
	FCombatAnimTableRow* CombatAnimRow = CombatAnimTable->FindRow<FCombatAnimTableRow>(ID, "");
	if (CombatAnimRow)
	{
		if (CombatAnimRow->RollDirectionAnim.Contains(Direction))
		{
			return CombatAnimRow->RollDirectionAnim[Direction];
		}
	}
	
	return nullptr;
}

UAnimMontage* UAnimManager::GetRollRandomDirection(FName ID) const
{
	FCombatAnimTableRow* CombatAnimRow = CombatAnimTable->FindRow<FCombatAnimTableRow>(ID, "");
	if (CombatAnimRow)
	{
	    TArray<ECharacterDirection> KeyArray;
        CombatAnimRow->RollDirectionAnim.GenerateKeyArray(KeyArray);
    
        int32 RandomIndex = FMath::RandRange(0, KeyArray.Num() - 1);
        ECharacterDirection RandomKey = KeyArray[RandomIndex];
        UAnimMontage* RandomValue = CombatAnimRow->RollDirectionAnim[RandomKey];
		if (RandomValue)
		{
			return RandomValue;
		}
	}
	
	return nullptr;
}

UAnimMontage* UAnimManager::GetHitInAir(FName ID) const
{
	FCombatAnimTableRow* CombatAnimRow = CombatAnimTable->FindRow<FCombatAnimTableRow>(ID, "");
	if (CombatAnimRow)
	{
		return CombatAnimRow->HitInAir;
	}

	return nullptr;
}

UAnimMontage* UAnimManager::GetTakeDownRecover(FName ID) const
{
	FCombatAnimTableRow* CombatAnimRow = CombatAnimTable->FindRow<FCombatAnimTableRow>(ID, "");
	if (CombatAnimRow)
	{
		return CombatAnimRow->Recover;
	}
	
	return nullptr;
}

UAnimMontage* UAnimManager::GetStun(FName ID) const
{
	FCombatAnimTableRow* CombatAnimRow = CombatAnimTable->FindRow<FCombatAnimTableRow>(ID, "");
	if (CombatAnimRow)
	{
		return CombatAnimRow->Stun;
	}

	return nullptr;
}
