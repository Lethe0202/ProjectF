// Fill out your copyright notice in the Description page of Project Settings.


#include "CooldownItemSlotWidget.h"

#include "Components/TextBlock.h"

void UCooldownItemSlotWidget::HandleUpdateItemCnt(int newItemCnt)
{
	if (newItemCnt > 0)
	{
		ItemCntText->SetText(FText::FromString(FString::FromInt(newItemCnt))); 	
	}
	else
	{
		ItemCntText->SetText(FText());
	}
}
