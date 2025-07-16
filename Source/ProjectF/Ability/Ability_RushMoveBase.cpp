// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_RushMoveBase.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "ProjectF/Common/CombatTypes.h"

bool UAbility_RushMoveBase::ActivateAbility()
{
	return Super::ActivateAbility();
}

void UAbility_RushMoveBase::AbilityEffect(int Index)
{
	Super::AbilityEffect(Index);
	
	if (Index == 0)
	{
		ACharacter* OwnerCharacter = Cast<ACharacter>(Owner);
		if (!OwnerCharacter || !OwnerCharacter->GetMesh()) return;
		
		UAnimInstance* CharacterAnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (!CharacterAnimInstance) return;
		
		OriginallyMovementSpeed = OwnerCharacter->GetMovementComponent()->GetMaxSpeed();
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed  = MovementSpeed;
		
		TimerStartTime = Owner->GetWorld()->GetTimeSeconds();
		Owner->GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &UAbility_RushMoveBase::HandleDashTimer, Owner->GetWorld()->GetDeltaSeconds(), true);
	}
	else if (Index == 1)
	{
		if (!EffectedActors.IsEmpty())
		{
			Owner->GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle);
			
			ACharacter* OwnerCharacter = Cast<ACharacter>(Owner);
			if (!OwnerCharacter || !OwnerCharacter->GetMesh()) return;
			
			if (AbilityDataAsset->AbilityAnim.IsValidIndex(2))
			{
				OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(AbilityDataAsset->AbilityAnim[2]);
			}
		}
	}
}

void UAbility_RushMoveBase::EndAbilityEffect(int Index)
{
	Super::EndAbilityEffect(Index);
}

void UAbility_RushMoveBase::HandleDashTimer()
{
	if (!Owner.IsValid())
	{
		Owner->GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle);
		return;
	}

	ACharacter* OwnerCharacter = Cast<ACharacter>(Owner);
	if (!OwnerCharacter)
	{
		Owner->GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle);
		return;
	}
	
	float ElpasedTime = Owner->GetWorld()->GetTimeSeconds() - TimerStartTime;
	if (ElpasedTime > DashDuration)
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = OriginallyMovementSpeed;
		UAnimMontage* ActivateMontage = OwnerCharacter->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage();
		OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("LoopEnd"), ActivateMontage);
		Owner->GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle);
	}
	
	OwnerCharacter->AddMovementInput(OwnerCharacter->GetActorForwardVector());
}