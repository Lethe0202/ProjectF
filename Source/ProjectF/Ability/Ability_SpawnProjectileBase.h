// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "Ability_SpawnProjectileBase.generated.h"

class AProjectileBase;

UENUM()
enum class EProjectileHitType
{
	Single,
	Range,
};

/**
 * 
 */
UCLASS()
class PROJECTF_API UAbility_SpawnProjectileBase : public UAbilityBase
{
	GENERATED_BODY()

public:
	virtual void AbilityEffect(int Index) override;

protected:
	virtual AProjectileBase* SpawnProjectile();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnHit(AActor* SelfActor, AActor* OtherActor, const FHitResult& Hit);

	void ApplySingleEffect(AActor* SelfActor, AActor* OtherActor, const FHitResult& Hit);
	void ApplyRangeEffect(AActor* SelfActor, AActor* OtherActor, const FHitResult& Hit);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile", meta=(AllowPrivateAccess=true))
	FName SpawnSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile", meta=(AllowPrivateAccess=true))
	FVector SpawnOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile", meta=(AllowPrivateAccess=true))
	EProjectileHitType ProjectileHitType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile", meta=(AllowPrivateAccess=true), meta=(EditCondition = "ProjectileHitType == EProjectileHitType::Range", EditConditionHides))
	float ProjectileHitRadius;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile", meta=(AllowPrivateAccess=true))
	TSubclassOf<AProjectileBase> ProjectileClass;
};
