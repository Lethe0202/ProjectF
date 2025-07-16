// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_StrongAttackVisualEffect.h"

#include "ProjectF/Ability/AbilityBase.h"
#include "ProjectF/Character/PFCharacterBase.h"
#include "ProjectF/Character/Component/AbilityComponent.h"
#include "ProjectF/Manager/CombatManager.h"
#include "ProjectF/Manager/PFGameInstance.h"


void UAnimNotify_StrongAttackVisualEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	APFCharacterBase* BaseCharacter = Cast<APFCharacterBase>(MeshComp->GetOwner());
	
	if (BaseCharacter != nullptr)
	{
		if (UAbilityComponent* AbilityComponent = BaseCharacter->GetComponentByClass<UAbilityComponent>())
		{
			TWeakObjectPtr<UAbilityBase> CurrentAbility = AbilityComponent->GetCurrentActivateAbility();
			if (!CurrentAbility.IsValid()) return;

			UAbilityDataAsset* AbilityDataAsset= CurrentAbility->GetAbilityDataAsset();
			if (AbilityDataAsset->EffectContainer.IsValidIndex(Index) == false) return;
			if (AbilityDataAsset->EffectContainer[Index].bStrongEffect == false) return;
			
			UPFGameInstance* PFGameInstance = Cast<UPFGameInstance>(BaseCharacter->GetWorld()->GetGameInstance());
			if (!PFGameInstance) return;
			
			UCombatManager* CombatManager = PFGameInstance->GetCombatManager();
			if (!CombatManager) return;
			
			CombatManager->StrongAttackVisualEffect(BaseCharacter);
		}
	}
}
