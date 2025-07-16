// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "PFAIController.generated.h"

class UBehaviorTreeComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Damage;
class APFCharacterBase;


UCLASS()
class PROJECTF_API APFAIController : public AAIController
{
	GENERATED_BODY()

public:
	APFAIController();
	virtual void Tick(float DeltaTime) override;
	
	void StartTree();
	
	virtual FGenericTeamId GetGenericTeamId() const override; //{ return static_cast<uint8>(TeamType); }
    virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void OnUpdatePerception(AActor* Actor, FAIStimulus Stimulus);
	
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AISetting, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AISetting, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAISenseConfig_Damage> DamageConfig;
	
	UPROPERTY()
	TObjectPtr<APFCharacterBase> PFOwnerCharacter;

	FGenericTeamId TeamId;
};
