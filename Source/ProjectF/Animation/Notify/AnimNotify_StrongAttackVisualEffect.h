// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_StrongAttackVisualEffect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTF_API UAnimNotify_StrongAttackVisualEffect : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	UPROPERTY(EditAnywhere, Category="AbilityInfo", meta = (EditCondition = "ActionEventType != EActionEventType::WeaponAttack", EditConditionHides))
	int32 Index;
};
