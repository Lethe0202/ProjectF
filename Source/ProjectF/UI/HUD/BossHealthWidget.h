// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProgressHUDWidget.h"
#include "BossHealthWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECTF_API UBossHealthWidget : public UProgressHUDWidget
{
	GENERATED_BODY()

public:
	void SetBossName(FString InBossName);
	
protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> BossName;
};
