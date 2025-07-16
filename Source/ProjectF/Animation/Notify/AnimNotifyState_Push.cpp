// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Push.h"

void UAnimNotifyState_Push::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	ElapsedTime = 0.f;
	Duration = TotalDuration;

	AActor* Owner = MeshComp->GetOwner();
	if (Owner)
	{
		InitialLocation = Owner->GetActorLocation();
		PushDirection = Owner->GetActorForwardVector().GetSafeNormal2D();
	}
}

void UAnimNotifyState_Push::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	
	ElapsedTime += FrameDeltaTime;
	float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.f, 1.f);

	AActor* Owner = MeshComp->GetOwner();
	if (Owner)
	{
		FVector NewLocation = InitialLocation + PushDirection * PushDistance * Alpha;
		NewLocation.Z = InitialLocation.Z;  // Z 유지
		
		Owner->SetActorLocation(NewLocation, true);
	}
}

void UAnimNotifyState_Push::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
}
