// Fill out your copyright notice in the Description page of Project Settings.


#include "ProgressHUDWidget.h"

#include "Components/ProgressBar.h"

void UProgressHUDWidget::OnProgressChanged_Implementation(float OldValue, float NewValue, float MaxValue, AActor* Instigator)
{
	ProgressBar->SetPercent(NewValue / MaxValue);
}
