// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExecutionProgressWidget.generated.h"

class UImage;
class UMaterialInstanceDynamic;

/**
 * 
 */
UCLASS()
class PROJECTF_API UExecutionProgressWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnUpdateHoldProgress(float MaxTime, float CurrentTime);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ProgressImage;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> ProgressMaterialInstance;
};
