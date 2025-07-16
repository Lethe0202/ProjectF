// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectF/Common/ResourceDelegates.h"
#include "HealthComponent.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FPFStatusChanged, float, OldValue, float, NewValue, float, MaxValue, AActor*, Instigator);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTF_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UHealthComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ApplyHeal(float Amount);
	void ApplyDamage(float Amount, AActor* Instigator);
	
protected:
	virtual void BeginPlay() override;
	
	void UpdaetHealth(int Amount, AActor* Instigator);
	
public:
	UPROPERTY(BlueprintAssignable)
	FPFStatusChanged OnHealthStatusChanged;
	
protected:
	UPROPERTY(EditAnywhere)
	float MaxHealth;
	
	float CurrentHealth;
};
