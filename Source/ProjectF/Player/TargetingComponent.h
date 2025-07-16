// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetingComponent.generated.h"


class APFCharacterBase;
class UWidgetComponent;
class UUserWidget;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PROJECTF_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTargetingComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SetupPlayerInputTargetingComponent(class ULocalPlayer* LocalPlayer, class UInputComponent* PlayerInputComponent);
	
	void ToggleTargeting();
	void LeftTargeting();
	void RightTargeting();

	bool IsActivateTargeting();

	AActor* GetTarget() const { return LockOnTarget; }
	void SetTarget(AActor* Target);
	
protected:
	virtual void BeginPlay() override;

	bool CheckTargeting(TArray<FHitResult>& OutHitResults);
	
	void UpdateNearestTargetActor();
	
	UFUNCTION()
	void UpdateLockOnCamera();

	void ResetCamera();
	
private:
	UPROPERTY()
	TObjectPtr<APFCharacterBase> OwnerCharacter;
	
	UPROPERTY()
	TObjectPtr<USpringArmComponent> OwnerCameraSpringArmComponent;
	
	UPROPERTY()
	TObjectPtr<UCameraComponent> OwnerCameraComponent;

	UPROPERTY()
	TObjectPtr<AActor> LockOnTarget;
	
	FTimerHandle LookOnTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "BaseSetting", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "BaseSetting", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> TargetingAction;

	UPROPERTY(EditDefaultsOnly, Category = "BaseSetting", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> TargetingMarkWidgetClass;
	
	UPROPERTY(EditDefaultsOnly,meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> TargetingWidgetComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "BaseSetting", meta = (AllowPrivateAccess = "true"))
	float CheckRangeSphereRadius;

	UPROPERTY(EditDefaultsOnly, Category = "BaseSetting", meta = (AllowPrivateAccess = "true"))
	float CameraInterpSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "BaseSetting", meta = (AllowPrivateAccess = "true"))
	float ClearLockOnDistance;
};
