// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AbilityEffect.h"

#include "ProjectF/Character/PFCharacterBase.h"
#include "ProjectF/Character/Component/AbilityComponent.h"


void UAnimNotify_AbilityEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (ActionEventType == EActionEventType::Ability)
	{
		APFCharacterBase* BaseCharacter = Cast<APFCharacterBase>(MeshComp->GetOwner());
		
		if (BaseCharacter != nullptr)
		{
			if (UAbilityComponent* AbilityComponent = BaseCharacter->GetComponentByClass<UAbilityComponent>())
			{
				AbilityComponent->AbilityEffect(AbilityClass, Index);
				AbilityComponent->EndAbilityEffect(AbilityClass, Index);
			}
		}
	}
}
