// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_PlayerCharge.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectF/Common/CombatTypes.h"
#include "ProjectF/Common/EffectTypes.h"

bool UAbility_PlayerCharge::ActivateAbility()
{
	return Super::ActivateAbility();
}

void UAbility_PlayerCharge::AbilityEffect(int Index)
{
	if (!Owner.Get())
	{
		return;
	}
	
	if (AbilityDataAsset->EffectContainer.IsValidIndex(Index))
	{
		TArray<AActor*> TargetActors;
		TArray<FHitResult> HitResult;
		AbilityDataAsset->EffectContainer[Index].TargetType->GetTargets(Owner.Get(), TargetActors, HitResult);
		
		for (auto& Item : EffectedActors)
		{
			TargetActors.Remove(Item);
		}
		
		for (auto& Item : TargetActors)
		{
			EffectedActors.Add(Item);
		}
		
		for (auto& Target : TargetActors)
		{
			for (const UEffectType* EffectType : AbilityDataAsset->EffectContainer[Index].Effect)
			{
				ACharacter* TargetCharacter = Cast<ACharacter>(Target);
				if (!TargetCharacter) return;
				
				if (TargetCharacter->GetCharacterMovement()->MovementMode == MOVE_Falling)
				{
					FVector XYLaunch = (TargetCharacter->GetActorLocation() - Owner->GetActorLocation()).GetSafeNormal() * 500.f;
					TargetCharacter->LaunchCharacter(FVector(XYLaunch.X, XYLaunch.Y, -3000.f), false, false);
					
					ACharacter* OwnerCharacter = Cast<ACharacter>(Owner);
					OwnerCharacter->GetCharacterMovement()->Velocity = FVector::ZeroVector;
					OwnerCharacter->GetCharacterMovement()->GravityScale = 0.1;
				}
				else
				{
					EffectType->ApplyEffect(Target, Owner.Get(), FTransform::Identity, AbilityDataAsset->EffectContainer[Index].bStrongEffect);
				}
			}
		}
	}
}
