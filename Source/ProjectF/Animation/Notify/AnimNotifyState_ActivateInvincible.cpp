// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_ActivateInvincible.h"

#include "ProjectF/Character/PFCharacterBase.h"
#include "ProjectF/Character/Component/CombatStateComponent.h"

void UAnimNotifyState_ActivateInvincible::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	APFCharacterBase* Character = Cast<APFCharacterBase>(Owner);
	if (!Character) return;

	if (UCombatStateComponent* CombatStateComponent = Character->GetCombatStateComponent())
	{
		CombatStateComponent->SetInvincible(true);
	}
}

void UAnimNotifyState_ActivateInvincible::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	APFCharacterBase* Character = Cast<APFCharacterBase>(Owner);
	if (!Character) return;

	if (UCombatStateComponent* CombatStateComponent = Character->GetCombatStateComponent())
	{
		CombatStateComponent->SetInvincible(false);
	}
}
