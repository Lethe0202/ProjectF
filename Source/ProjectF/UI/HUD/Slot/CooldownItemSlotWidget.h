// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CooldownSlotWidget.h"
#include "CooldownItemSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTF_API UCooldownItemSlotWidget : public UCooldownSlotWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void HandleUpdateItemCnt(int newItemCnt);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemCntText;
};
