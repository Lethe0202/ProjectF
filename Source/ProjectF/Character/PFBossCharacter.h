// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PFAICharacter.h"
#include "PFBossCharacter.generated.h"

class UBossPhaseDataAsset;
class ULevelSequencePlayer;

UCLASS()
class PROJECTF_API APFBossCharacter : public APFAICharacter
{
	GENERATED_BODY()

public:
	APFBossCharacter();
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	virtual void OnChangedPhase();
	
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBossPhaseDataAsset> BossPhaseDataAsset;

	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> LevelSequencePlayer;

	UPROPERTY(EditAnywhere)
	FName DisplayName;
};
