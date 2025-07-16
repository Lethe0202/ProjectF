// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../State/ICombatState.h"
#include "CombatStateComponent.generated.h"

class APFCharacterBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PROJECTF_API UCombatStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatStateComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	bool ChangeState(ECombatState NewState);

	UFUNCTION(BlueprintCallable)
	bool CanChangeState(ECombatState NewState);

	void ActivateCounter(AActor* Target, FDamageEvent const& DamageEvent);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayCounterEffect(AActor* Target);
	
protected:
	virtual void BeginPlay() override;

	template<typename NewStateType>
	void ChangeToState();
	
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
public:
	UFUNCTION(BlueprintCallable)
	ECombatState GetCurrentState() { return CombatState.Get()->GetStateType();}
	
	bool GetInvincible() const { return bInvincible; }
	bool GetIsCountering() const { return bIsCountering; }
	bool GetSuperArmor() const { return bSuperArmor; }
	bool IsGuardCounter() const { return bGuardCounter; }
	
	UFUNCTION(BlueprintCallable)
	void SetInvincible(bool bInInvincible) { bInvincible = bInInvincible; }
	
	UFUNCTION(BlueprintCallable)
	void SetCountering(bool bInCountering) { bIsCountering = bInCountering; }

	UFUNCTION(BlueprintCallable)
	void SetSuperArmor(bool bInSuperArmor) { bSuperArmor = bInSuperArmor; }
	
	UFUNCTION(BlueprintCallable)
	void SetGuardCounter(bool bInGuardCounter) { bGuardCounter = bInGuardCounter; }
	
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ConuterParticleSystem;

	UPROPERTY()
	TObjectPtr<APFCharacterBase> OwnerCharacter;
	
	TUniquePtr<ICombatState> CombatState;

	// 회피
	bool bInvincible = false;

	// 회피 카운터중
	bool bIsCountering = false;

	UPROPERTY(EditAnywhere)
	bool bSuperArmor = false;
	
	// 가드 카운터 가능
	bool bGuardCounter = false;
	
	bool bDodging = false;
};
