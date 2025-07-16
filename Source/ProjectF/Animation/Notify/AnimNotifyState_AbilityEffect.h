// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotify_AbilityEffect.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_AbilityEffect.generated.h"

class UAbilityBase;

/**
 * 
 */
UCLASS()
class PROJECTF_API UAnimNotifyState_AbilityEffect : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	UPROPERTY(EditAnywhere, Category="AbilityInfo")
	EActionEventType ActionEventType;
		
	UPROPERTY(EditAnywhere, Category="AbilityInfo")
	int32 EffectIndex;
	
	UPROPERTY(EditAnywhere, Category="AbilityInfo", meta = (EditCondition = "ActionEventType == EActionEventType::Ability", EditConditionHides))
	TSubclassOf<UAbilityBase> AbilityClass;
	
	UPROPERTY(EditAnywhere, Category="AbilityInfo", meta = (EditCondition = "ActionEventType == EActionEventType::Ability", EditConditionHides))
	int NotifyCheckCnt = 0;

	float StateTotalDuraion = 0.f;

	float ClearTimingValue = 0.f;

	float NotifyDuration = 0.f;
	float CurrentTiming = 0.f;
};
