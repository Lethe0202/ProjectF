// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProgressHUDWidget.generated.h"

class UProgressBar;
class UHealthComponent;


UCLASS()
class PROJECTF_API UProgressHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void OnProgressChanged(float OldValue, float NewValue, float MaxValue, AActor* Instigator);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar;
};
