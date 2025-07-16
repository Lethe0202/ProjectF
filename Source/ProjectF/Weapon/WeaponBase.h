// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class UAbilityDataAsset;
class UAbilityBase;
class USkeletalMeshComponent;
class UNiagaraComponent;

UENUM(BlueprintType)
enum class EWeaponAttackType : uint8
{
	None,
	Primary,
	Strong
};

UCLASS()
class PROJECTF_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:
	AWeaponBase();
	virtual void Tick(float DeltaTime) override;

	bool TryAttack();
	bool TryStrongAttack();
	bool TryChargeAttack();
	void SaveAttack();
	void ResetAttack();

	bool TryGuard();
	bool ReleaseGuard();
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	void SetChargeEffect(bool bCharge);
	void ActivateWeaponCollision(bool bCollision, int EffectIndex = 0);
	
	const int GetGroundAttackCnt() const { return AttackCnt; }
	UAnimMontage* GetGuardSuccessMontage() const { return GuardSuccessMontage; }
	UAnimMontage* GetGuardCounterMontage();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystemComponent> WeaponChargeParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(allowPrivateAccess=true))
	TObjectPtr<UShapeComponent> ShapeComponent; 

	/*
	 * Default Ability
	 */
	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<UAbilityBase> ChargeAttackAbilityClass;
	
	UPROPERTY()
	TObjectPtr<UAbilityBase> ChargeAttackAbility;
	
	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<UAbilityBase> LaunchAttackAbilityClass;
	
	UPROPERTY()
	TObjectPtr<UAbilityBase> LaunchAttackAbility;
	
	UPROPERTY(EditAnywhere, Category = "Ability")
	TObjectPtr<UAnimMontage> ChargeAttackMontage;
	
	UPROPERTY(EditAnywhere, Category = "Ability")
	TObjectPtr<UAnimMontage> LaunchAttackMontage;

	/*
	 * Guard
	 */
	UPROPERTY(EditAnywhere, Category = "Guard")
	TObjectPtr<UAnimMontage> GuardMontage;

	UPROPERTY(EditAnywhere, Category = "Guard")
	TObjectPtr<UAnimMontage> GuardSuccessMontage;
	
	UPROPERTY(EditAnywhere, Category = "Guard")
	TArray<TObjectPtr<UAnimMontage>> GuardCounterMontageArray;
	
	int32 CurrentGuardCounterIndex = 0; 

	EWeaponAttackType CurrentWeaponAttackType;
	
	int32 AttackCnt = 0;
	int32 StrongAttackCnt = 0;
	int32 JumpAttackCnt = 0;
	int32 CurrentEffectIndex = -1;
	bool bCanLaunchAttack = true;
	bool bCanChargeAttack = true;
	bool bStrongAttack = false;

	UPROPERTY()
	TArray<class APFCharacterBase*> HitCharacters;
	
	UPROPERTY(EditAnywhere, Category = "Ability")
	TObjectPtr<UAbilityDataAsset> PrimaryAttackDataAsset;
	
	UPROPERTY(EditAnywhere, Category = "Ability")
	TObjectPtr<UAbilityDataAsset> StrongAttackDataAsset;
	
	UPROPERTY(EditAnywhere, Category = "Ability")
	TObjectPtr<UAbilityDataAsset> JumpAttackDataAsset;
};
