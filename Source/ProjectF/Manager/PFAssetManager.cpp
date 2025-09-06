// Fill out your copyright notice in the Description page of Project Settings.


#include "PFAssetManager.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

void UPFAssetManager::RegisterLoadDefaultAssets()
{
	UAssetManager& Manager = UAssetManager::Get();
    Manager.RefreshPrimaryAssetDirectory(true);

    TArray<FAssetData> AssetDataList;
	
	Manager.GetPrimaryAssetDataList(FPrimaryAssetType("AbilityData"), AssetDataList);
	for (const FAssetData& AssetData : AssetDataList)
	{
		PrimaryAssetIds.Add(AssetData.GetPrimaryAssetId());
	}
	AssetDataList.Empty();
	
	Manager.GetPrimaryAssetDataList(FPrimaryAssetType("LevelData"), AssetDataList);
	for (const FAssetData& AssetData : AssetDataList)
	{
		PrimaryAssetIds.Add(AssetData.GetPrimaryAssetId());
	}
	AssetDataList.Empty();
	
	Manager.GetPrimaryAssetDataList(FPrimaryAssetType("FX"), AssetDataList);
	for (const FAssetData& AssetData : AssetDataList)
	{
		PrimaryAssetIds.Add(AssetData.GetPrimaryAssetId());
	}
}

void UPFAssetManager::RegisterLoadAssetByType(const FName AssetType)
{
	UAssetManager& Manager = UAssetManager::Get();
	Manager.RefreshPrimaryAssetDirectory(true);

	TArray<FAssetData> AssetDataList;
	
	Manager.GetPrimaryAssetDataList(FPrimaryAssetType(AssetType), AssetDataList);
	for (const FAssetData& AssetData : AssetDataList)
	{
		PrimaryAssetIds.Add(AssetData.GetPrimaryAssetId());
	}
}

void UPFAssetManager::RegisterLoadAssetByID(const FPrimaryAssetId ID)
{
	PrimaryAssetIds.Add(ID);
}

void UPFAssetManager::StartLoadAsset(const FSimpleDelegate& OnLoaded)
{
	UAssetManager& Manager = UAssetManager::Get();
	bLoadingAsset = true;
	
	LoadHandle = Manager.LoadPrimaryAssets(PrimaryAssetIds.Array(), {},  FStreamableDelegate::CreateLambda([this, OnLoaded]()
	{
		UE_LOG(LogTemp, Log, TEXT("로드 완료"));
		bLoadingAsset = false;
		
		if (OnLoaded.IsBound())
		{
			OnLoaded.Execute();
		}
	}));
}

float UPFAssetManager::GetLoadingProgress() const
{
	if (LoadHandle)
	{
		return LoadHandle->GetProgress();
	}
	
	return 1.f;
}
