// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PFCharacterMovementComponent.generated.h"


class APFCharacterBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTF_API UPFCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UPFCharacterMovementComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void PhysFalling(float deltaTime, int32 Iterations) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void PhysWalking(float deltaTime, int32 Iterations) override;

protected:
	void PlayMontageTakeDownRecover();
	
private:
	UPROPERTY()
	TObjectPtr<APFCharacterBase> OwnerCharacter;

	float TakeDownDurationTime = 0.f;
	float InAirDurationTime = 0.f;
};
