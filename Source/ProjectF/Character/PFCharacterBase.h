// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "ProjectF/Common/CombatTypes.h"
#include "PFCharacterBase.generated.h"

class AWeaponBase;
class UCombatStateComponent;
class UAbilityComponent;
class UHealthComponent;
class UStaminaComponent;

UENUM()
enum class ETeamType : uint8
{
	Player = 1,
	Enemy = 2,
	None = 255,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterInit);

UCLASS()
class PROJECTF_API APFCharacterBase : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	APFCharacterBase();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/*
	 * Combat
	 */
	virtual bool TryAttack();
	virtual void AttackHold(float HoldTime);
	virtual bool TryAttackHold(float HoldTime);

	UFUNCTION(BlueprintCallable)
	virtual void SaveAttack();
	
	UFUNCTION(BlueprintCallable)
	virtual void ResetAttack();
	
	UFUNCTION(BlueprintCallable)
	bool IsWeaopnAttachedHand();
	
	UFUNCTION(BlueprintCallable)
	virtual void EquipWeapon(bool bEquip);

	virtual void ActivateAbility(int Index);
	virtual void ActivateAbility(TSubclassOf<UAbilityBase> AbilityClass);
	
	void InterruptAction();

	virtual void RollCharacter(ECharacterDirection Direction);
	
	UFUNCTION(BlueprintCallable)
	bool ChangeState(ECombatState NewState);
	
	UFUNCTION(BlueprintCallable)
	bool CanChangeState(ECombatState NewState);
	
	virtual void StopLogic(FString Reason);

	// 다른 캐릭터에게 Execution 당할 시 호출 
	virtual void Executed();

	// Execution 행위자의 타이밍을 위한 함수
	virtual void OnExecutionTrigger();
	
	// Execution 행위자가 호출 / 당하는사람 호출 X
	virtual void OnExecutionEnd();

	// Execution 행위자가 액션 끝났을때, Execution 대상자 호출
	virtual void OnExecutedEnd();

	UFUNCTION(BlueprintCallable)
	ECombatState GetCurrentState() const;
	
	AWeaponBase* GetWeapon() const { return Weapon; }
	UCombatStateComponent* GetCombatStateComponent() const { return CombatStateComponent; }
	
	void SetCanAttack(bool bInCanAttack);
	/*
	* Combat End 
	*/

	/*
	 * Status
	 */
	virtual float ApplyStamina(float InStamina);
	
	void SetControlledMovement(bool bControlledMovement);
	
protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void ModifyDamage(float DamageAmount, AActor* InInstigator);
	
	/*
	* Combat 
	*/
	virtual void SuccessGuardCounter(AActor* InInstigator);
	virtual void SuccessGuard(float InStaminaValue);
	virtual void SuccessAbilityCounter(); 

	UFUNCTION()
	void ResetParryInvincible();

public:
	FName GetID() const { return ID; }
	virtual FGenericTeamId GetGenericTeamId() const override; //{ return static_cast<uint8>(TeamType); }
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION(BlueprintCallable)
	ETeamAttitude::Type GetTeamAttitudeTowardsBP(const AActor* Other) const;
	
	UAbilityComponent* GetAbilityComponent() const { return AbilityComponent; }
	UHealthComponent* GetHealthComponent() const { return HealthComponent; }
	UStaminaComponent* GetStaminaComponent() const { return StaminaComponent; }
	virtual AActor* GetTarget() const { return nullptr; }
	bool IsGuardCounterSuccess() const { return bParrySuccess; }

	FOnCharacterInit CharacterInitDelegate;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCombatStateComponent> CombatStateComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UHealthComponent> HealthComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaminaComponent> StaminaComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilityComponent> AbilityComponent;
	
	UPROPERTY(EditAnywhere)
	FName ID;
	
	UPROPERTY(EditAnywhere)
	ETeamType TeamType;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeaponBase> StartWeaponClass;
	
	UPROPERTY()
	TObjectPtr<AWeaponBase> Weapon;
	
	bool bCanAttack = true;
	bool bStrongAttack = false; 
	
	UPROPERTY(EditAnywhere)
	float ChargeHoldTime = 1.f;

	// 대미지 받은 이후에 parry 성공하면 State가 Idle로 변경되는데 이후 추가적인 효과(Stagger등)을 무시하기 위한 플래그
	bool bParrySuccess = false;
};
