// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_AbilityEffect.h"

#include "ProjectF/Character/PFCharacterBase.h"
#include "ProjectF/Character/Component/AbilityComponent.h"
#include "ProjectF/Weapon/WeaponBase.h"

void UAnimNotifyState_AbilityEffect::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	NotifyDuration = 0.f;
	StateTotalDuraion = TotalDuration;
	ClearTimingValue = StateTotalDuraion / NotifyCheckCnt;
	CurrentTiming = ClearTimingValue;
	
	if (ActionEventType == EActionEventType::WeaponAttack)
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			if (APFCharacterBase* Character = Cast<APFCharacterBase>(Owner))
			{
				if (!Character->GetWeapon()) return;
				
				Character->GetWeapon()->ActivateWeaponCollision(true, EffectIndex);
			}
		}
	}
}

void UAnimNotifyState_AbilityEffect::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	if (!MeshComp)
	{
		return;
	}
	
	if (ActionEventType == EActionEventType::Ability)
	{
		APFCharacterBase* BaseCharacter = Cast<APFCharacterBase>(MeshComp->GetOwner());
		
		if (BaseCharacter != nullptr)
		{
			if (UAbilityComponent* AbilityComponent = BaseCharacter->GetComponentByClass<UAbilityComponent>())
			{
				AbilityComponent->AbilityEffect(AbilityClass, EffectIndex);
			}
		}
	}
	
	if (NotifyCheckCnt == 0)
	{
		return;
	}
	
	NotifyDuration += FrameDeltaTime;
	
	if (CurrentTiming <= NotifyDuration)
	{
		if (ActionEventType == EActionEventType::Ability)
		{
			APFCharacterBase* BaseCharacter = Cast<APFCharacterBase>(MeshComp->GetOwner());
		
			if (BaseCharacter != nullptr)
			{
				if (UAbilityComponent* AbilityComponent = BaseCharacter->GetComponentByClass<UAbilityComponent>())
				{
					AbilityComponent->EndAbilityEffect(AbilityClass, EffectIndex);
				}
			}
		}
		
		CurrentTiming += ClearTimingValue;
	}
}

void UAnimNotifyState_AbilityEffect::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (ActionEventType == EActionEventType::WeaponAttack)
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			if (APFCharacterBase* Character = Cast<APFCharacterBase>(Owner))
			{
				Character->GetWeapon()->ActivateWeaponCollision(false, EffectIndex);
			}
		}
	}
	else if (ActionEventType == EActionEventType::Ability)
	{
		APFCharacterBase* BaseCharacter = Cast<APFCharacterBase>(MeshComp->GetOwner());
		
		if (BaseCharacter != nullptr)
		{
			if (UAbilityComponent* AbilityComponent = BaseCharacter->GetComponentByClass<UAbilityComponent>())
			{
				AbilityComponent->EndAbilityEffect(AbilityClass, EffectIndex);
			}
		}
	}
}