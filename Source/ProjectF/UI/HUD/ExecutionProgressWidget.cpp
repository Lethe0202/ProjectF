// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecutionProgressWidget.h"
#include "Components/Image.h"

void UExecutionProgressWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ProgressMaterialInstance = ProgressImage->GetDynamicMaterial();
}

void UExecutionProgressWidget::OnUpdateHoldProgress(float MaxTime, float CurrentTime)
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
	}
	else
	{
		ProgressMaterialInstance->SetScalarParameterValue("Progress", CurrentTime / MaxTime);
		SetVisibility(ESlateVisibility::Visible);
	}
}
