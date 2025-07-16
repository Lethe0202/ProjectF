// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"


class UAbilityDataAsset;
class APFCharacterBase;
class UInputAction;
class UInputMappingContext;
class UAbilityBase;

USTRUCT()
struct FAbilityCooldownInfo
{
	GENERATED_BODY()

public:
	FTimerHandle TimerHandle;
	float Cooldown;
	float CooldownStartTime;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnUpdateHoldProgressDelegate, float, MaxTime, float, CurrentTime, float, PerfectMaxTime, float, PerfectMinTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpdateCooldownDelegate, int, Index, float, MaxCooldown, float, CurrentCooldown);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTF_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UAbilityComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnHoldingAbility(int Index, float HoldingTime);
	void OnReleasedAbility(int Index, float HoldingTime);
	
	bool ActivateAbility(int Index);
	bool ActivateAbility(TSubclassOf<UAbilityBase> AbilityClass);
	void AbilityEffect(TSubclassOf<UAbilityBase> AbilityClass, int Index);
	void EndAbilityEffect(TSubclassOf<UAbilityBase> AbilityClass, int Index);
	void RegisterAbility(UAbilityBase* Ability);
	void UnregisterAbility(UAbilityBase* Ability);
	
	void EndAbility();
	void StopAbility();
	void ClearAbility();
	
	TWeakObjectPtr<UAbilityBase> GetCurrentActivateAbility() const { return CurrentActivateAbility; };
	UAbilityBase* GetAbilityByIndex(int Index) { return Abilities.IsValidIndex(Index) ? Abilities[Index] : nullptr; };
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void Input_ActivateAbility(TSubclassOf<UAbilityBase> AbilityClass);
	
	bool CanActivateAbility(int Index);
	
	void ApplyCooldown(int Index);

	UFUNCTION()
	void HandleCooldownUpdate(int Index);
	
public:
	FOnUpdateHoldProgressDelegate OnUpdateHoldProgress;
	FOnUpdateCooldownDelegate OnUpdateCooldown;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UAbilityBase>> AbilitiesClass;
	
	UPROPERTY()
	TArray<TObjectPtr<UAbilityBase>> Abilities;
	
	UPROPERTY()
	TWeakObjectPtr<UAbilityBase> CurrentActivateAbility;
	
	UPROPERTY()
	TArray<FAbilityCooldownInfo> AbilityCooldownInfo;

	UPROPERTY()
	TObjectPtr<APFCharacterBase> OwnerCharacter;
};


