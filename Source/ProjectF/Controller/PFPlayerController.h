// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "PFPlayerController.generated.h"

class UTargetingComponent;
class APFPlayerCharacter;
class UInputMappingContext;
class UInputAction;

class UPlayerHUDWidget;
class UDialogueWidget;

/**
 * 
 */
UCLASS()
class PROJECTF_API APFPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	UFUNCTION()
	void HandleCharacterInit();

	/*
	 * Input
	 */
	void Input_Targeting();
	void Input_LeftTarget();
	void Input_RightTarget();
	
	void Input_Interaction();
	
public:
	/*
	 * Interaction
	 */
	void ShowDialogueUI(FName DialogueID);
	void HiddenDialogueUI();
	
	UFUNCTION(BlueprintCallable)
	void AddWidgetToHUDSlot(FName SlotName, UUserWidget* Widget);
	
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
protected:
	UFUNCTION()
	void HandleDialogueEnd();
	
protected:
	UPROPERTY()
	TObjectPtr<APFPlayerCharacter> OwnerCharacter;

	FGenericTeamId TeamId;
	
protected:
	/*
     * Targeting
     */
	UPROPERTY(EditDefaultsOnly, Category = "Input | Targeting", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> TargetingMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input | Targeting", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> TargetingAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input | Targeting", meta = (AllowPrivateAccess = true))
    TObjectPtr<UInputAction> LeftTargetAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input | Targeting", meta = (AllowPrivateAccess = true))
    TObjectPtr<UInputAction> RightTargetAction;
	
	UPROPERTY()
	TObjectPtr<UTargetingComponent> OwnerTargetingComponent;

	/*
	* System
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Input | System", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> SystemMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input | System", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> InteractionAction;
	
	/*
	 * Interaction
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Input | Targeting", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> InteractionMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input | Targeting", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> NextDialogueAction;
	
	/*
	 * Main HUD
	 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerHUDWidget> PlayerHUDWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UPlayerHUDWidget> PlayerHUDWidget;
	
	/*
	 * Runtime UI Panel
	 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> DialogueWidgetClass;

	UPROPERTY()
	TObjectPtr<UDialogueWidget> DialogueWidget;
};












