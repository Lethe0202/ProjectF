﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE()
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTF_API IInteractable
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void VisibleInteractablePrompt(bool bVisible);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(AActor* InteractingActor);
};
