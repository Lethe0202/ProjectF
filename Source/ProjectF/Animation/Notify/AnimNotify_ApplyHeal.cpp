// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ApplyHeal.h"

#include "ProjectF/Character/PFPlayerCharacter.h"

void UAnimNotify_ApplyHeal::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (APFPlayerCharacter* PlayerCharacter = Cast<APFPlayerCharacter>(MeshComp->GetOwner()))
    {
		PlayerCharacter->ApplyHeal();
    }
}
