#pragma once

#include "EffectTypes.generated.h"

class UHitSFXDataAsset;

USTRUCT(BlueprintType)
struct FEffectInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FHitResult Hit;

	UPROPERTY(EditAnywhere)
	FTransform EffectTransform = FTransform();
	
	UPROPERTY(EditAnywhere)
	FVector HitNormal = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere)
	bool bStrongEffect = false;
};

UCLASS(Abstract, EditInlineNew, Blueprintable)
class UEffectType : public UObject
{
	GENERATED_BODY()

public:
	UEffectType() {}

	UFUNCTION(BlueprintNativeEvent)
	void ApplyEffect(AActor* Target, AActor* EffectCauser, FEffectInfo EffectInfo = FEffectInfo(), bool bStrongEffect = false) const;

protected:
	bool CanApplyEffect(AActor* Target) const;
};

UCLASS(EditInlineNew)
class UEffectType_Damage : public UEffectType
{
	GENERATED_BODY()
	
public:
	UEffectType_Damage() {}
	virtual void ApplyEffect_Implementation(AActor* Target, AActor* EffectCauser, FEffectInfo EffectInfo = FEffectInfo(), bool bStrongEffect = false) const override;
	
protected:
	UPROPERTY(EditAnywhere)
	float Damage;
};

UCLASS(EditInlineNew)
class UEffectType_HitEffect : public UEffectType
{
	GENERATED_BODY()
	
public:
	UEffectType_HitEffect() {}
	virtual void ApplyEffect_Implementation(AActor* Target, AActor* EffectCauser, FEffectInfo EffectInfo = FEffectInfo(), bool bStrongEffect = false) const override;
	
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ParticleSystem;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UHitSFXDataAsset> HitSFXDataAsset;
};

UCLASS(EditInlineNew)
class UEffectType_BeamEffect : public UEffectType
{
	GENERATED_BODY()
	
public:
	UEffectType_BeamEffect() {}
	virtual void ApplyEffect_Implementation(AActor* Target, AActor* EffectCauser, FEffectInfo EffectInfo = FEffectInfo(), bool bStrongEffect = false) const override;
	
protected:
	UPROPERTY(EditAnywhere)
	FName SocketName;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ParticleSystem;
};


UCLASS(EditInlineNew)
class UEffectType_Stagger : public UEffectType
{
	GENERATED_BODY()
	
public:
	UEffectType_Stagger() {}
	virtual void ApplyEffect_Implementation(AActor* Target, AActor* EffectCauser, FEffectInfo EffectInfo = FEffectInfo(), bool bStrongEffect = false) const override;
	
protected:
	UPROPERTY(EditAnywhere)
	FVector Power;
};

UCLASS(Blueprintable, EditInlineNew)
class UEffectType_Launch : public UEffectType
{
	GENERATED_BODY()

public:
	UEffectType_Launch() {}
	virtual void ApplyEffect_Implementation(AActor* Target, AActor* EffectCauser, FEffectInfo EffectInfo = FEffectInfo(), bool bStrongEffect = false) const override;
	
protected:
	void StrongEffect(AActor* Target, AActor* EffectCauser, FTransform EffectTransform) const;
	void NormalEffect(AActor* Target, AActor* EffectCauser, FTransform EffectTransform) const;
	
protected:
	UPROPERTY(EditAnywhere)
	FVector Power;
	
	UPROPERTY(EditAnywhere)
	bool bChangestate = true;
};

UCLASS()
class UStrongDamageType : public UDamageType
{
	GENERATED_BODY()
	
};