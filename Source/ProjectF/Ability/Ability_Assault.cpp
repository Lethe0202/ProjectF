// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Assault.h"

#include "GameFramework/Character.h"
#include "ProjectF/Common/CombatTypes.h"

bool UAbility_Assault::ActivateAbility()
{
	return Super::ActivateAbility();
}

void UAbility_Assault::AbilityEffect(int Index)
{
	Super::AbilityEffect(Index);
	
	if (Index == 1)
	{
		ACharacter* OwnerCharacter = Cast<ACharacter>(Owner);
		if (!OwnerCharacter) return;
		if (bCycle == true) return;
		
		bCycle = true;
		OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(AbilityDataAsset->AbilityAnim[0]);
	}
	else if (Index == 2)
	{
		bLastStrongAttack = true;
	}
}

void UAbility_Assault::HoldEnd(float HoldingTime)
{
	Super::HoldEnd(HoldingTime);

	ACharacter* OwnerCharacter = Cast<ACharacter>(Owner);
	if (!OwnerCharacter) return;
	if (bLastStrongAttack == true) return;
	
	OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Stop(0.25, AbilityDataAsset->AbilityAnim[0]);

	EndAbility();
}

void UAbility_Assault::EndAbility()
{
	Super::EndAbility();
	bCycle = false;
	bLastStrongAttack = false;
}
