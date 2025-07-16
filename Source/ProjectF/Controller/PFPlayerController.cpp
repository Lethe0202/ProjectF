// Fill out your copyright notice in the Description page of Project Settings.


#include "PFPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectF/Character/PFPlayerCharacter.h"
#include "ProjectF/Interaction/Interactable.h"
#include "ProjectF/Player/TargetingComponent.h"
#include "ProjectF/UI/HUD/PlayerHUDWidget.h"
#include "ProjectF/UI/HUD/Dialogue/DialogueWidget.h"

class UEnhancedInputLocalPlayerSubsystem;

void APFPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (Cast<APFPlayerCharacter>(InPawn))
	{
		OwnerCharacter = Cast<APFPlayerCharacter>(InPawn);
		OwnerCharacter->CharacterInitDelegate.AddDynamic(this, &APFPlayerController::HandleCharacterInit);

		OwnerTargetingComponent = InPawn->FindComponentByClass<UTargetingComponent>();
	}
	
	TeamId = OwnerCharacter->GetGenericTeamId();
}

void APFPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void APFPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(this))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(TargetingMappingContext, 0);
			Subsystem->AddMappingContext(SystemMappingContext, 0);
		}
	
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			EnhancedInputComponent->BindAction(TargetingAction, ETriggerEvent::Started, this, &APFPlayerController::Input_Targeting);
			EnhancedInputComponent->BindAction(LeftTargetAction, ETriggerEvent::Started, this, &APFPlayerController::Input_LeftTarget);
			EnhancedInputComponent->BindAction(RightTargetAction, ETriggerEvent::Started, this, &APFPlayerController::Input_RightTarget);
			
			EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &APFPlayerController::Input_Interaction);
		}
	}
}

void APFPlayerController::HandleCharacterInit()
{
	PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(this, PlayerHUDWidgetClass);
	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->AddToViewport();
		PlayerHUDWidget->InitPlayerHUDWidget(OwnerCharacter);	
	}
}

void APFPlayerController::Input_Targeting()
{
	if (!OwnerTargetingComponent)
	{
		if (GetPawn())
		{
			OwnerTargetingComponent = GetPawn()->FindComponentByClass<UTargetingComponent>();
		}
	}
	
	if (!OwnerTargetingComponent) return;
	
	OwnerTargetingComponent->ToggleTargeting();

	if (OwnerTargetingComponent->IsActivateTargeting())
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 450.f;	
	}
	else
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
}

void APFPlayerController::Input_LeftTarget()
{
	if (!OwnerTargetingComponent)
	{
		if (GetPawn())
		{
			OwnerTargetingComponent = GetPawn()->FindComponentByClass<UTargetingComponent>();
		}
	}

	if (!OwnerTargetingComponent) return;

	OwnerTargetingComponent->LeftTargeting();
}

void APFPlayerController::Input_RightTarget()
{
	if (!OwnerTargetingComponent)
	{
		if (GetPawn())
		{
			OwnerTargetingComponent = GetPawn()->FindComponentByClass<UTargetingComponent>();
		}
	}
	
	if (!OwnerTargetingComponent) return;
	
	OwnerTargetingComponent->RightTargeting();
}

void APFPlayerController::Input_Interaction()
{
	OwnerCharacter->TryInteraction();
}

void APFPlayerController::ShowDialogueUI(FName DialogueID)
{
	if (DialogueWidget)
	{
		DialogueWidget->InitDialogue(DialogueID);
		DialogueWidget->SetVisibility(ESlateVisibility::Visible);				
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
	else
	{
		if (DialogueWidgetClass)
		{
			DialogueWidget = CreateWidget<UDialogueWidget>(this, DialogueWidgetClass);
			
			if (DialogueWidget && DialogueWidget->GetClass()->ImplementsInterface(UHUDPanelSlotWidgetInterface::StaticClass()))
			{
				FName HUDSlotName = IHUDPanelSlotWidgetInterface::Execute_GetHUDSlotName(DialogueWidget);
				PlayerHUDWidget->AddWidgetToSlot(HUDSlotName, DialogueWidget);
				DialogueWidget->OnDialogueEnd.AddDynamic(this, &APFPlayerController::HandleDialogueEnd);
				DialogueWidget->InitDialogue(DialogueID);
				
				SetInputMode(FInputModeUIOnly());
				bShowMouseCursor = true;
			}
		}
	}
}

void APFPlayerController::HiddenDialogueUI()
{
	if (DialogueWidget)
	{
		DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
}

void APFPlayerController::AddWidgetToHUDSlot(FName SlotName, UUserWidget* Widget)
{
	PlayerHUDWidget->AddWidgetToSlot(SlotName, Widget);
}

void APFPlayerController::HandleDialogueEnd()
{
	HiddenDialogueUI();
}

FGenericTeamId APFPlayerController::GetGenericTeamId() const
{
	return TeamId;
}

ETeamAttitude::Type APFPlayerController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);
	if (!OtherTeamAgent)
    {
    	return ETeamAttitude::Neutral;
    }
	
	if (GetGenericTeamId() != OtherTeamAgent->GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Friendly;
}