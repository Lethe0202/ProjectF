#pragma once

#include "Kismet/KismetSystemLibrary.h"
#include "TargetTypes.generated.h"


UCLASS(Abstract, EditInlineNew)
class UTargetType : public UObject
{
	GENERATED_BODY()

public:
	UTargetType() {}
	
	UFUNCTION(BlueprintNativeEvent)
	void GetTargets(AActor* TargetingActor, TArray<AActor*>& OutActors, TArray<FHitResult>& OutHitResults) const;
};

UCLASS()
class UTargetType_Self : public UTargetType
{
	GENERATED_BODY()

public:
	UTargetType_Self() {}
	
	virtual void GetTargets_Implementation(AActor* TargetingActor, TArray<AActor*>& OutActors, TArray<FHitResult>& OutHitResults) const override;
};

UCLASS(Blueprintable)
class UTargetType_Sphere : public UTargetType
{
	GENERATED_BODY()

public:
	UTargetType_Sphere() {}
	
	virtual void GetTargets_Implementation(AActor* TargetingActor, TArray<AActor*>& OutActors, TArray<FHitResult>& OutHitResults) const override;
	
protected:
	UPROPERTY(EditAnywhere)
	FName SocketName;

	UPROPERTY(EditAnywhere)
	FName EndSocketName;
	
	UPROPERTY(EditAnywhere)
	FVector Offset;
	
	UPROPERTY(EditAnywhere)
	float Radius;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDebug = false;
	
	UPROPERTY(EditAnywhere, Category = "Debug", meta = (EditCondition = "bDebug", EditConditionHides))
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugTrace;
	
	UPROPERTY(EditAnywhere, Category = "Debug", meta = (EditCondition = "bDebug", EditConditionHides))
	float Duration = 0.f;
};

UCLASS()
class UTargetType_Targeting : public UTargetType
{
	GENERATED_BODY()

public:
	UTargetType_Targeting() {}
	
	virtual void GetTargets_Implementation(AActor* TargetingActor, TArray<AActor*>& OutActors, TArray<FHitResult>& OutHitResults) const override;
};

