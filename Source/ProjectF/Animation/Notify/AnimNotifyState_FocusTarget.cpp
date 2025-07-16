// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_FocusTarget.h"

#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectF/Character/PFCharacterBase.h"

void UAnimNotifyState_FocusTarget::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	APFCharacterBase* OwnerCharacter = Cast<APFCharacterBase>(MeshComp->GetOwner());
	if (!OwnerCharacter) return;
	
	AActor* TargetActor = OwnerCharacter->GetTarget();
	if (!TargetActor) return;

	AAIController* AIController = Cast<AAIController>(OwnerCharacter->GetController());
	if (!AIController) return;

	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	OwnerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	OwnerCharacter->bUseControllerRotationYaw = true;
				
	AIController->SetFocus(TargetActor);
}

void UAnimNotifyState_FocusTarget::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	APFCharacterBase* OwnerCharacter = Cast<APFCharacterBase>(MeshComp->GetOwner());
	if (!OwnerCharacter) return;

	AAIController* AIController = Cast<AAIController>(OwnerCharacter->GetController());
	if (!AIController) return;
	
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
    		
    	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
    	OwnerCharacter->bUseControllerRotationYaw = false;
    	OwnerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = true;
}
