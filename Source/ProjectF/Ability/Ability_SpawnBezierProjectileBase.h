// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability_SpawnProjectileBase.h"
#include "Ability_SpawnBezierProjectileBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTF_API UAbility_SpawnBezierProjectileBase : public UAbility_SpawnProjectileBase
{
	GENERATED_BODY()
	
public:

protected:
	virtual AProjectileBase* SpawnProjectile();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile | Bezier")
	float Duration;
};
