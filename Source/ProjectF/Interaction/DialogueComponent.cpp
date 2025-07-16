// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueComponent.h"

#include "ProjectF/Controller/PFPlayerController.h"

UDialogueComponent::UDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDialogueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDialogueComponent::StartDialogue(AActor* InteractingActor)
{
	APawn* InteractingPawn = Cast<APawn>(InteractingActor);
	if (!InteractingPawn) return;

	APFPlayerController* PFPlayerController = nullptr;
	if (InteractingPawn->Controller)
	{
		PFPlayerController= Cast<APFPlayerController>(InteractingPawn->Controller);
	}
	
	if (!PFPlayerController) return;

	PFPlayerController->ShowDialogueUI(DialogueID);
}

