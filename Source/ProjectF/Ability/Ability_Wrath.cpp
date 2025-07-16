// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Wrath.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "ProjectF/Character/PFCharacterBase.h"
#include "ProjectF/Character/State/ICombatState.h"
#include "ProjectF/Common/CombatTypes.h"

bool UAbility_Wrath::ActivateAbility()
{
	if (AbilityDataAsset->bHolding)
	{
		if (bHoldActivate == true)
		{
			return false;	
		}
	}
	
	AbilityDataAsset->AbilityAnim[0]->bEnableAutoBlendOut = false;
	AbilityDataAsset->AbilityAnim[2]->bEnableAutoBlendOut = false;
	
	APFCharacterBase* PFCharacter = Cast<APFCharacterBase>(Owner);
	if (!PFCharacter) return false;
	if (!PFCharacter->CanChangeState(ECombatState::Attacking)) return false;
	
	if (PFCharacter->GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		if (!AbilityDataAsset) return false;
		UAnimInstance* CharacterAnimInstance = PFCharacter->GetMesh()->GetAnimInstance();
		if (!CharacterAnimInstance) return false;
		
		FHitResult HitResult;
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(Owner.Get());

		FCollisionShape Shape = FCollisionShape::MakeSphere(45.f);
		bool bHit = Owner->GetWorld()->SweepSingleByChannel(HitResult, Owner->GetActorLocation(),
			Owner->GetActorLocation() + FVector(0.f, 0.f, -10000.f),FQuat::Identity, ECC_Pawn, Shape, CollisionQueryParams);

		if (!bHit) return false;
		
		FVector DistanceToGround = Owner->GetActorLocation() - HitResult.ImpactPoint;
		if (DistanceToGround.Z < 200.f) return false;
		
		EffectedActors.Empty();
		bLaunch = false;
		CharacterAnimInstance->Montage_Play(AbilityDataAsset->AbilityAnim[2]);
		CacheCharacter = PFCharacter;
		
		PFCharacter->ChangeState(ECombatState::Attacking);
		if (ACharacter* Character = Cast<ACharacter>(Owner))
		{
			Character->GetCharacterMovement()->GravityScale = 1.5f;
		}
		
		GetWorld()->GetTimerManager().SetTimer(CheckGroundTimerHandle, this, &UAbility_Wrath::OnCheckGroundTimer, GetWorld()->GetDeltaSeconds(), true, -1.f);
		return true;
	}
	else
	{
		return Super::ActivateAbility();
	}
}

void UAbility_Wrath::HoldEnd(float HoldingTime)
{
	if (!bHoldActivate) return;
	
	bHoldActivate = false;
	
	ACharacter* Character = Cast<ACharacter>(Owner);
	if (!Character || !Character->GetMesh()) return;
	
	UAnimInstance* CharacterAnimInstance = Character->GetMesh()->GetAnimInstance();
	if (!CharacterAnimInstance) return;
	
	if (Character->GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		return;
	}
	
	float MinPerfectAlpha = AbilityDataAsset->MinPerfectTiming / AbilityDataAsset->MaxHoldTime;
	float MaxPerfectAlpha = AbilityDataAsset->MaxPerfectTiming / AbilityDataAsset->MaxHoldTime;
	
	if (AbilityDataAsset->MinPerfectTiming < HoldingTime && HoldingTime < AbilityDataAsset->MaxPerfectTiming)
	{
		if (AbilityDataAsset->AbilityAnim.IsValidIndex(1))
		{
			CharacterAnimInstance->Montage_Play(AbilityDataAsset->AbilityAnim[1]);
		}
	}
	else
	{
		if (AbilityDataAsset->AbilityAnim.IsValidIndex(0))
		{
			AbilityDataAsset->AbilityAnim[0]->bEnableAutoBlendOut = true;
			CharacterAnimInstance->Montage_Play(AbilityDataAsset->AbilityAnim[0]);
			CharacterAnimInstance->Montage_JumpToSection("HoldEnd", AbilityDataAsset->AbilityAnim[0]);
		}
	}
}

void UAbility_Wrath::OnCheckGroundTimer()
{
	if (Owner.IsValid())
	{
		FHitResult HitResult;
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(Owner.Get());

		FCollisionShape Shape;
		Shape = FCollisionShape::MakeSphere(45.f);
		
		bool bHit = Owner->GetWorld()->SweepSingleByChannel(
			HitResult, Owner->GetActorLocation(), Owner->GetActorLocation() + FVector(0.f, 0.f, -10000.f),
			FQuat::Identity, ECC_Pawn, Shape, CollisionQueryParams);

		if (bHit)
		{
			FVector DistanceToGround = Owner->GetActorLocation() - HitResult.ImpactPoint;
			if (DistanceToGround.Z < 10.f + 90.f)
			{
				if (UAnimInstance* CharacterAnimInstance = CacheCharacter->GetMesh()->GetAnimInstance())
				{
					AbilityDataAsset->AbilityAnim[2]->bEnableAutoBlendOut = true;
					CharacterAnimInstance->Montage_Play(AbilityDataAsset->AbilityAnim[2]);
					CharacterAnimInstance->Montage_JumpToSection("OnGround", AbilityDataAsset->AbilityAnim[2]);

					AbilityEffect(1);
					
					Owner->GetWorld()->GetTimerManager().ClearTimer(CheckGroundTimerHandle);
				}
			}
		}
	}
}