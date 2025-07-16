// Fill out your copyright notice in the Description page of Project Settings.


#include "BossHealthWidget.h"

#include "Components/TextBlock.h"

void UBossHealthWidget::SetBossName(FString InBossName)
{
	BossName->SetText(FText::FromString(InBossName));
}
