// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HUDPanelSlotWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UHUDPanelSlotWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTF_API IHUDPanelSlotWidgetInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FName GetHUDSlotName() const; 
};
