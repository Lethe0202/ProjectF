#pragma once

#include "EffectTypes.generated.h"

UCLASS(Abstract, EditInlineNew)
class UEffectType : public UObject
{
	GENERATED_BODY()

public:
	UEffectType() {}
	
	virtual void ApplyEffect(AActor* Target, AActor* EffectCauser, FTransform EffectTransform = FTransform(), bool bStrongEffect = false) const;
};

UCLASS(EditInlineNew)
class UEffectType_Damage : public UEffectType
{
	GENERATED_BODY()
	
public:
	UEffectType_Damage() {}
	virtual void ApplyEffect(AActor* Target, AActor* EffectCauser, FTransform EffectTransform = FTransform(), bool bStrongEffect = false) const override;
	
protected:
	UPROPERTY(EditAnywhere)
	float Damage;
};

UCLASS(EditInlineNew)
class UEffectType_Stamina : public UEffectType
{
	GENERATED_BODY()
	
public:
	UEffectType_Stamina() {}
	virtual void ApplyEffect(AActor* Target, AActor* EffectCauser, FTransform EffectTransform = FTransform(), bool bStrongEffect = false) const override;
	
protected:
	UPROPERTY(EditAnywhere)
	float Stamina;
	
	UPROPERTY(EditAnywhere)
	bool bOnlyGuarded = true;
};

UCLASS(EditInlineNew)
class UEffectType_HitEffect : public UEffectType
{
	GENERATED_BODY()
	
public:
	UEffectType_HitEffect() {}
	virtual void ApplyEffect(AActor* Target, AActor* EffectCauser, FTransform EffectTransform = FTransform(), bool bStrongEffect = false) const override;
	
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ParticleSystem;
};

UCLASS(EditInlineNew)
class UEffectType_BeamEffect : public UEffectType
{
	GENERATED_BODY()
	
public:
	UEffectType_BeamEffect() {}
	virtual void ApplyEffect(AActor* Target, AActor* EffectCauser, FTransform EffectTransform = FTransform(), bool bStrongEffect = false) const override;
	
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
	virtual void ApplyEffect(AActor* Target, AActor* EffectCauser, FTransform EffectTransform = FTransform(), bool bStrongEffect = false) const override;
	
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
	virtual void ApplyEffect(AActor* Target, AActor* EffectCauser, FTransform EffectTransform = FTransform(), bool bStrongEffect = false) const override;
	
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