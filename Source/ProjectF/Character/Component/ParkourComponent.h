// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ParkourComponent.generated.h"

class ACharacter;
class UCharacterMovementComponent;
class UCombatStateComponent;
class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PROJECTF_API UParkourComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UParkourComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintNativeEvent)
	void StartParkour(AActor* TargetActor, FHitResult& HitResult);
	
protected:
	UPROPERTY()
	TObjectPtr<ACharacter> Owner;

	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;

	UPROPERTY()
	UCombatStateComponent* CombatStateComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> ClimbMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> MantleMontage;
};
