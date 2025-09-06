// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundManager.h"

#include "Components/AudioComponent.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectF/Common/LevelDataAsset.h"

void USoundManager::PlayBGM(FString LevelName, UWorld* World)
{
	FString LevelPath = FString::Printf(TEXT("DA_%s"), *LevelName);
	
	FPrimaryAssetId LevelAssetId = FPrimaryAssetId("LevelData", FName(*LevelPath));
	ULevelDataAsset* LevelData = Cast<ULevelDataAsset>(UAssetManager::Get().GetPrimaryAssetObject(LevelAssetId));
	
	if (!LevelData) return;
	
	if (!BGMComponent)
	{
		BGMComponent = UGameplayStatics::CreateSound2D(World, LevelData->LevelBGM);
		BGMComponent->Play();
	}
	else
	{
		BGMComponent->SetSound(LevelData->LevelBGM);
		BGMComponent->Play();
	}
}
