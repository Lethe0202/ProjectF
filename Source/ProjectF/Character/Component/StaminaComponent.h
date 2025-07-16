// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectF/Common/ResourceDelegates.h"
#include "StaminaComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTF_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStaminaComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddStamina(float InValue);
	
protected:
	virtual void BeginPlay() override;

	void UpdateStamina(float InValue);

public:
	float GetCurrentStamina() const { return CurrentStamina; }

public:
	UPROPERTY(BlueprintAssignable)
	FPFStatusChanged OnStaminaChanged;
	
protected:
	UPROPERTY(EditAnywhere)
	float MaxStamina = 100.f;

	float CurrentStamina = 0.f;
	
	//Per seconds
	UPROPERTY(EditAnywhere)
	float AutoRecoveryValue = 0.f;
};
