// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityBase.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectF/Character/PFPlayerCharacter.h"
#include "ProjectF/Character/State/ICombatState.h"
#include "ProjectF/Common/CombatTypes.h"
#include "ProjectF/Common/EffectTypes.h"

#

UAbilityBase::UAbilityBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	if (AActor* Actor = Cast<AActor>(GetOuter()))
	{
		Owner = MakeWeakObjectPtr<AActor>(Actor);	
	}
}

bool UAbilityBase::ActivateAbility()
{
	APFCharacterBase* PFCharacter = Cast<APFCharacterBase>(Owner);
	if(PFCharacter)
	{
		if (!PFCharacter->CanChangeState(ECombatState::Attacking))
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	if (PFCharacter->GetMovementComponent()->IsFalling())
	{
		if (!AbilityDataAsset->bCanUseInAir)
		{
			return false;
		}
	}
	
	EffectedActors.Empty();
	bLaunch = false;
	
	if (!AbilityDataAsset)
	{
		return false;
	}
	
	if (AbilityDataAsset->AbilityAnim.IsValidIndex(0))
	{
		if (PFCharacter->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage() == nullptr)
		{
			PFCharacter->ChangeState(ECombatState::Attacking);
			PFCharacter->GetMesh()->GetAnimInstance()->Montage_Play(AbilityDataAsset->AbilityAnim[0]);
		}
		else
		{
			return false;
		}
	}

	if (AbilityDataAsset->bHolding)
	{
		bHoldActivate = true;		
	}
	
	return true;
}

void UAbilityBase::AbilityEffect(int Index)
{
	if (!Owner.Get()) return;
	
	if (AbilityDataAsset->EffectContainer.IsValidIndex(Index) && AbilityDataAsset->EffectContainer[Index].TargetType != nullptr)
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
			EffectedActors.AddUnique(Item);
		}
		
		IGenericTeamAgentInterface* OwnerTeamAgent = Cast<IGenericTeamAgentInterface>(Owner.Get());
		if (!OwnerTeamAgent) return;
		
		for (int i = 0; i < TargetActors.Num(); ++i)
		{
			if (TargetActors[i] != Owner.Get())
			{
				if (OwnerTeamAgent->GetTeamAttitudeTowards(*TargetActors[i]) != ETeamAttitude::Hostile) continue;	
			}
			
			for (const UEffectType* EffectType : AbilityDataAsset->EffectContainer[Index].Effect)
			{
				FTransform EffectTransform;
				if (HitResult.IsValidIndex(i))
				{
					EffectTransform.SetLocation(HitResult[i].ImpactPoint);
				}
				
				EffectType->ApplyEffect(TargetActors[i], Owner.Get(), EffectTransform, AbilityDataAsset->EffectContainer[Index].bStrongEffect);
			}
		}
	}
	
	if ((Cast<ACharacter>(Owner)->GetCharacterMovement()->MovementMode == MOVE_Falling))
	{
		if (!bLaunch)
		{
			ACharacter* Character = Cast<ACharacter>(Owner.Get());
			if (Character)
			{
				bLaunch = true;
			}
		}
	}

	if (bMultiHit)
	{
		EffectedActors.Empty();
	}
}

void UAbilityBase::EndAbilityEffect(int Index)
{
	EffectedActors.Empty();
}

void UAbilityBase::HoldEnd(float HoldingTime)
{
	return;
}

void UAbilityBase::EndAbility()
{
	if (AbilityDataAsset->bHolding)
	{
		bHoldActivate = false;
	}
}