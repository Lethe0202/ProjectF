// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EQSContext_Target.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTF_API UEQSContext_Target : public UEnvQueryContext
{
	GENERATED_BODY()

public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
	
protected:
	UPROPERTY(EditAnywhere)
	FName BlackboardKeyName;
};
