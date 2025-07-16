// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ExecutionEnd.h"

#include "ProjectF/Character/PFCharacterBase.h"

void UAnimNotify_ExecutionEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (APFCharacterBase* Character = Cast<APFCharacterBase>(MeshComp->GetOwner()))
	{
		Character->OnExecutionEnd();
	}
}
