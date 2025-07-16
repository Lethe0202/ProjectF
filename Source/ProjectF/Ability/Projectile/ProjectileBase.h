// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnProjectileEffectSignature, AActor*, SelfActor, AActor*, OtherActor,const FHitResult&, Hit);

UCLASS()
class PROJECTF_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBase();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnProjectileEffectSignature OnProjectileEffect;
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
};
