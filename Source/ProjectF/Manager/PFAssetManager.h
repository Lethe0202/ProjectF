// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/StreamableManager.h"
#include "PFAssetManager.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECTF_API UPFAssetManager : public UObject
{
	GENERATED_BODY()

public:
	void RegisterLoadDefaultAssets();
	void RegisterLoadAssetByType(const FName AssetType);
	void RegisterLoadAssetByID(const FPrimaryAssetId ID);

	void StartLoadAsset(const FSimpleDelegate& OnLoaded);
	
	UFUNCTION(BlueprintCallable)
	float GetLoadingProgress() const;

	UFUNCTION(BlueprintCallable)
	int GetLoadCnt() const { return PrimaryAssetIds.Num(); }
	
protected:
	TSet<FPrimaryAssetId> PrimaryAssetIds;

	TSharedPtr<FStreamableHandle> LoadHandle;
	bool bLoadingAsset;
};
