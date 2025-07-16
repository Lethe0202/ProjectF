// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_AerialFall.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectF/Character/PFPlayerCharacter.h"

bool UAbility_AerialFall::ActivateAbility()
{
	return Super::ActivateAbility();
}

void UAbility_AerialFall::AbilityEffect(int Index)
{
	Super::AbilityEffect(Index);
	
	if (Index == 4)
	{
		ACharacter* Character = Cast<ACharacter>(Owner);
		if (!Character || !Character->GetMesh()) return;
		
		UAnimInstance* CharacterAnimInstance = Character->GetMesh()->GetAnimInstance();
		if (!CharacterAnimInstance) return;
		
		if (AbilityDataAsset->AbilityAnim.IsValidIndex(1))
		{
			CharacterAnimInstance->Montage_Play(AbilityDataAsset->AbilityAnim[1]);
		}
	}
}

void UAbility_AerialFall::EndAbilityEffect(int Index)
{
	Super::EndAbilityEffect(Index);
}
