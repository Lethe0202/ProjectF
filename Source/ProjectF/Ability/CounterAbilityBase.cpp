// Fill out your copyright notice in the Description page of Project Settings.


#include "CounterAbilityBase.h"

#include "GameFramework/Character.h"
#include "ProjectF/Character/PFCharacterBase.h"
#include "ProjectF/Common/CombatTypes.h"
#include "ProjectF/Manager/AnimManager.h"
#include "ProjectF/Manager/PFGameInstance.h"

void UCounterAbilityBase::AbilityEffect(int Index)
{
	Super::AbilityEffect(Index);
	
	if (!AbilityDataAsset) return;
	if (!AbilityDataAsset->CounterSuccessAnim) return;
	if (!Owner.IsValid()) return;
	
	if (Index == 2)
	{
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(Owner.Get()))
		{
			if (AbilityDataAsset->AbilityAnim.IsValidIndex(1))
			{
				OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(AbilityDataAsset->AbilityAnim[1]);	
			}
		}
		
		UGameInstance* GameInstance = GetWorld()->GetGameInstance();
		if (!GameInstance) return;

		UPFGameInstance* PFGameInstance = Cast<UPFGameInstance>(GameInstance);
		if (!PFGameInstance) return;
		
		for (auto& EffectedActor : EffectedActors)
		{
			if (APFCharacterBase* PFCharacterBase = Cast<APFCharacterBase>(EffectedActor))
			{
				if (UAnimMontage* AnimMontage = PFGameInstance->GetAnimManager()->GetHitDirection(PFCharacterBase->GetID(), ECharacterDirection::Forward))
				{
					PFCharacterBase->GetMesh()->GetAnimInstance()->Montage_Play(AnimMontage);	
				}
			}
		}
	}
}

void UCounterAbilityBase::CounterEffect()
{
	if (!AbilityDataAsset) return;
	if (!AbilityDataAsset->CounterSuccessAnim) return;
	if (!Owner.IsValid()) return;
	
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(Owner.Get()))
	{
		OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection("CounterSuccess");
	}
}
