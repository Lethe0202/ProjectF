// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilityHoldProgressHUDWidget.generated.h"


class UImage;

UCLASS()
class PROJECTF_API UAbilityHoldProgressHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnUpdateHoldProgress(float MaxTime, float CurrentTime, float PerfectMaxTime, float PerfectMinTime);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ProgressImage;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> ProgressMaterialInstance;
};
