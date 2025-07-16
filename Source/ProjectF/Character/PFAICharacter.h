// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PFCharacterBase.h"
#include "ProjectF/Interaction/Interactable.h"
#include "PFAICharacter.generated.h"

class UWidgetComponent;
class UBehaviorTree;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FExecutionCooldownSignature, float, MaxCooldown, float, CurrentCooldown);

UCLASS()
class PROJECTF_API APFAICharacter : public APFCharacterBase, public IInteractable
{
	GENERATED_BODY()
	
public:
	APFAICharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// IInteractable Interface
	virtual void Interact_Implementation(AActor* InteractingActor) override;
	// ~Interactable Interface

	virtual void StopLogic(FString Reason) override;

	/*
	 * Combat
	 */
	virtual void Executed() override;
	virtual void OnExecutedEnd() override;
	
protected:
	virtual void BeginPlay() override;

	virtual float ApplyStamina(float InStamina) override;
	
	UFUNCTION()
	void HandleExecutionCooldownTimer();
	
	virtual void ModifyDamage(float DamageAmount, AActor* InInstigator) override;
	
public:
	UBehaviorTree* const GetBT() const { return (BehaviorTree != nullptr ? BehaviorTree : nullptr); }
	virtual AActor* GetTarget() const;

	FExecutionCooldownSignature ExecutionCooldownDelegate;
	
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> ExecutionWidgetComponent;

	/*
	 * Execution
	 */
	FTimerHandle ExecutionCooldownTimer;
	float ExecutionCooldownStartTime;
	bool bExecuted = false;
};
