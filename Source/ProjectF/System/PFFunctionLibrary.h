// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PFFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTF_API UPFFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="ProjectF")
	static UWorld* GetActorWorld (const AActor* Actor);
};
