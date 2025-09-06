// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_Push.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTF_API UAnimNotifyState_Push : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Push", meta = (AllowPrivateAccess = "true"))
	float PushDistance = 500.f;
	
	FVector InitialLocation;
	FVector PushDirection;
	float ElapsedTime;
	float Duration;

};
