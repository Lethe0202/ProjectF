// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "BezierProjectileBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTF_API ABezierProjectileBase : public AProjectileBase
{
	GENERATED_BODY()

public:
	ABezierProjectileBase();
	
	UFUNCTION(BlueprintCallable)
	void InitBezierProjectile(FTransform InTargetTransform, float InDuration);

protected:
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bezier")
	float CurveHeight = 100.f;

	// 0.0 ~ 1.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bezier")
	float MidPointAlpha = 0.5f;
	
	FTransform TargetTransform;
	float Duration;
	float ElpasedTime;
	FVector LastFrameVelocity;

	bool bSimulate = false;
	
	// 시작지점
	FVector P0;
	
	// 중간 지점
	FVector P1;
	
	// 끝지점
	FVector P2;
};


