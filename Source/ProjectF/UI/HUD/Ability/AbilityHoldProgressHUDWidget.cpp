// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityHoldProgressHUDWidget.h"

#include "Components/Image.h"

void UAbilityHoldProgressHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ProgressMaterialInstance = ProgressImage->GetDynamicMaterial();
	SetVisibility(ESlateVisibility::Collapsed);
}

void UAbilityHoldProgressHUDWidget::OnUpdateHoldProgress(float MaxTime, float CurrentTime, float PerfectMaxTime, float PerfectMinTime)
{
	if (!ProgressMaterialInstance)
	{
		ProgressMaterialInstance = ProgressImage->GetDynamicMaterial();
		if (!ProgressMaterialInstance)
		{
			return;
		}
	}
	
	if (MaxTime == 0.f)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	else
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	
	ProgressMaterialInstance->SetScalarParameterValue("Progress", CurrentTime / MaxTime);
	ProgressMaterialInstance->SetScalarParameterValue("MaxPerfectTiming", PerfectMaxTime / MaxTime);
	ProgressMaterialInstance->SetScalarParameterValue("MinPerfectTiming", PerfectMinTime / MaxTime);
}
