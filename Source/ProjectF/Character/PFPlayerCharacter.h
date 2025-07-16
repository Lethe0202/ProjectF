// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PFCharacterBase.h"
#include "InputActionValue.h"
#include "ProjectF/Common/CombatTypes.h"
#include "PFPlayerCharacter.generated.h"

class UWidgetComponent;
class USphereComponent;
class UInputAction;
class UInputMappingContext;

class UTargetingComponent;
class USpringArmComponent;
class UCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdatePotionCooldownSignature, float, MaxCooldown, float, CurrentCooldown);

UCLASS()
class PROJECTF_API APFPlayerCharacter : public APFCharacterBase
{
	GENERATED_BODY()
	
public:
	APFPlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/*
	* Movement
	*/
	void Input_Move(const FInputActionValue& Value);
	void Input_Look(const FInputActionValue& Value);
	void Input_Roll(const FInputActionValue& Value);
	void Input_Jump(const FInputActionValue& Value);
	void Input_JumpStop(const FInputActionValue& Value);
	
	/*
	 * Ability
	 */
	void Input_Attack(const FInputActionValue& Value);
	void Input_StrongAttack(const FInputActionValue& Value);
	void Input_AttackHold(const FInputActionValue& Value);
	void Input_AttackHoldStop(const FInputActionValue& Value);

	void Input_AbilityPressed(int Index);
	void Input_AbilityHold(int Index);
	void Input_AbilityReleased(int Index);
	
	virtual bool TryAttack() override;
	bool TryStrongAttack();
	virtual void RollCharacter(ECharacterDirection Direction) override;
	
	void OnHoldingAbility(int Index, float HoldingTime);
	void OnReleasedAbility(int Index, float HoldingTime);
	
	/*
	 * Combat
	 */
	void Input_Guard(const FInputActionValue& Value);
	void Input_GuardReleased(const FInputActionValue& Value);
	void Input_Execution(const FInputActionValue& Value);
	
	UFUNCTION()
	void HandleExecutionCollisionTimer();

	virtual void OnExecutionTrigger() override;
	virtual void OnExecutionEnd() override;
	
	/*
	 * Status
	 */
	void Input_Heal();
	void ApplyHeal();
	virtual float ApplyStamina(float InStamina) override;
	virtual void ModifyDamage(float DamageAmount, AActor* InInstigator) override;

	/*
	 * Camera
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ChangeCameraMode(EDynamicCameraType DynamicCameraType);
	
	virtual AActor* GetTarget() const override;

	/*
	 * Utility
	 */
	void RotateMovement();

	/*
	 * Interaction
	 */
	bool TryInteraction();
	
	UFUNCTION()
	void HandleInteractionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleInteractionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	/*
	 * Delegate
	 */
	FOnUpdatePotionCooldownSignature OnUpdatePotionCooldown;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleUpdatePotionCooldown();

	/*
	 * Combat | Guard
	 */
	virtual void SuccessGuardCounter(AActor* InInstigator) override;
	virtual void SuccessGuard(float InStaminaValue) override;
	virtual void SuccessAbilityCounter() override;
	
protected:
	/*
	 * Movement
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Default", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RollAction;
	
	FVector CurMovementDirection;
	
	/*
	 * Combat
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> ExecutionInteractionWidgetComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> GuardAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> StrongAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ExecutionAction;
	
	UPROPERTY()
	TObjectPtr<APFCharacterBase> ExecutionTargetCharacter;

	FTimerHandle ExecutionTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | Execution")
	TObjectPtr<UAnimMontage> ExecutionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | Execution")
	TObjectPtr<UAbilityDataAsset> ExecutionDataAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | Execution")
	TObjectPtr<UParticleSystem> ExecutionHitEffect;

	bool bPlayExecution = false;

	/*
	 * Combat | Guard
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | Guard")
	TSubclassOf<UAbilityBase> GuardCounterAbilityClass;
	
	UPROPERTY()
	TObjectPtr<UAbilityBase>  GuardCounterAbility;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | Guard")
	TObjectPtr<UAnimMontage> GuardCounterMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | Guard")
	float GuardCounterAttackDuration = 0.25f;

	UPROPERTY()
	AActor* CurrentCounterTarget;
	
	bool bCanGuardCounterAttack = false;
	FTimerHandle GuardCounterAttackTimerHandle;
	
	/*
	 * Ability
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Ability")
	TObjectPtr<UInputMappingContext> AbilityInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Ability")
	TObjectPtr<UInputAction> Ability1Action;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Ability")
	TObjectPtr<UInputAction> Ability2Action;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Ability")
	TObjectPtr<UInputAction> Ability3Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Ability")
	TObjectPtr<UInputAction> Ability4Action;

	/*
	 * Heal
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input | Heal")
	TObjectPtr<UInputAction> HealAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Heal")
	float HealingItemAmount = 30.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Heal")
	float HealingItemCooldown = 20.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Heal")
	TObjectPtr<UAnimMontage> HealingMontage;

	float PotionCooldownStartTime = 0.f;
	FTimerHandle HealingCooldownTimerHandle;
	
private:
	float AttackHoldTime = 0.f;

	UPROPERTY()
	TMap<int, FAbilityInputData> AbilityInputDataMap;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTargetingComponent> TargetingComponent;

	/*
	 * Interaction
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> InteractionSphereComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> InteractableActors;
};


