// Fill out your copyright notice in the Description page of Project Settings.


#include "BezierProjectileBase.h"

#include "GameFramework/ProjectileMovementComponent.h"

ABezierProjectileBase::ABezierProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMovementComponent->bSimulationEnabled = false;
}

void ABezierProjectileBase::InitBezierProjectile(FTransform InTargetTransform, float InDuration)
{
	TargetTransform = InTargetTransform;
	Duration = InDuration;

	P0 = GetActorLocation();
	P2 = TargetTransform.GetLocation();
	
	P1 = P0 + (P2 - P0) * MidPointAlpha;
	
	float MaxZ = FMath::Max(P0.Z, P2.Z);
	P1.Z = MaxZ + CurveHeight;
	
	FVector Direction = (P2 - P0).GetSafeNormal();
	FVector Up = FVector::UpVector;
	FVector Right = FVector::CrossProduct(Direction, Up).GetSafeNormal();
	
	P1 += Right *  FMath::RandRange(-500.f, 500.f); 
	
	bSimulate = true;
}

void ABezierProjectileBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bSimulate)
	{
		ElpasedTime += DeltaSeconds;
		float t = ElpasedTime / Duration;
		t = FMath::Clamp(t, 0.f, 1.f);
		
		if (t >= 1.f)
		{
			ProjectileMovementComponent->MaxSpeed = LastFrameVelocity.Size() / DeltaSeconds;
			ProjectileMovementComponent->Velocity = LastFrameVelocity.GetSafeNormal() * ProjectileMovementComponent->MaxSpeed;
			ProjectileMovementComponent->bSimulationEnabled = true;
		}
		else
		{
			// from https://en.wikipedia.org/wiki/B%C3%A9zier_curve
			FVector NewPos = (1-t) * (1-t) * P0 + 2 * (1-t) * t * P1 + t * t * P2;
			FVector Delta = NewPos - GetActorLocation();
			LastFrameVelocity = Delta;
			
			AddActorWorldOffset(Delta, true);
			SetActorRotation(Delta.Rotation());
		}
	}
}
