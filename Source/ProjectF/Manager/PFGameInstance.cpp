// Fill out your copyright notice in the Description page of Project Settings.


#include "PFGameInstance.h"

#include "AnimManager.h"
#include "CombatManager.h"
#include "DialogueManager.h"
#include "EnemyManager.h"
#include "ResourceManager.h"
#include "SaveManager.h"
#include "SoundManager.h"
#include "Blueprint/UserWidget.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectF/Character/PFPlayerCharacter.h"

void UPFGameInstance::Init()
{
	Super::Init();

	if (AnimManagerClass)
	{
		AnimManager = NewObject<UAnimManager>(this, AnimManagerClass);
		if (AnimManager)
		{
			AnimManager->InitAnimManager();
		}
	}

	if (DialogueManagerClass)
	{
		DialogueManager = NewObject<UDialogueManager>(this, DialogueManagerClass);
	}
	
	if (CombatManagerClass)
	{
		CombatManager = NewObject<UCombatManager>(this, CombatManagerClass);
	}
	
	if (ResourceManagerClass)
	{
		ResourceManager = NewObject<UResourceManager>(this, ResourceManagerClass);
	}

	if (EnemyManagerClass)
	{
		EnemyManager = NewObject<UEnemyManager>(this, EnemyManagerClass);
		if (EnemyManager)
		{
			EnemyManager->Init();
		}
	}

	if (SaveManagerClass)
	{
		SaveManager = NewObject<USaveManager>(this, SaveManagerClass);
		if (SaveManager)
		{
			TSet<FString> DeadEnemyID = SaveManager->GetEnemySpawnSave();

			if (EnemyManager)
			{
				EnemyManager->SetDeadEnemyID(DeadEnemyID);
			}
		}
	}
	
	if (SoundManagerClass)
	{
		SoundManager = NewObject<USoundManager>(this, SoundManagerClass);
	}

	if (PFAssetManagerClass)
	{
		PFAssetManager = NewObject<UPFAssetManager>(this, PFAssetManagerClass);
		if (PFAssetManager)
		{
			PFAssetManager->RegisterLoadDefaultAssets();
		}
	}
	
	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UPFGameInstance::HandlePostWorldInitialization);
}

void UPFGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UPFGameInstance::ReadyPlayerCharacter(APFPlayerCharacter* PlayerCharacter)
{
	PlayBGM(PlayerCharacter->GetWorld());
	
	if (bDebugMode)
	{
		return;
	}
	
	if (PlayerCharacter)
	{
		APFPlayerCharacter* PFPlayerCharacter = Cast<APFPlayerCharacter>(PlayerCharacter);
		if (PFPlayerCharacter)
		{
			SaveManager->LoadPlayerInfo(PFPlayerCharacter);				
		}
	}
}

void UPFGameInstance::ChangeLevel(const TSoftObjectPtr<UWorld> NewLevel)
{
	if (bPreOpenLevel) return;

	UAssetManager& Manager = UAssetManager::Get();
	Manager.RefreshPrimaryAssetDirectory(true);
	
	bPreOpenLevel = true;
	TargetLevelPath = NewLevel.GetLongPackageName();
	
	// 로딩 레벨로 이동 후, 1초 후 package load
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPFGameInstance::HandleStartLoading, 1.0f, false);
}

void UPFGameInstance::SaveGame(APFPlayerCharacter* PlayerCharacter)
{
	if (SaveManager)
	{
		if (EnemyManager)
		{
			SaveManager->SaveEnemySpawn(EnemyManager->GetDeadEnemyID());	
		}
		
		if (PlayerCharacter)
		{
			SaveManager->SavePlayerInfo(PlayerCharacter);
		}
	}
}

void UPFGameInstance::LoadGame()
{
	FString LevelPath = SaveManager->GetSaveLevelPath();
	
	if (bPreOpenLevel) return;

	bPreOpenLevel = true;
	TargetLevelPath = LevelPath;

	if (SaveManager && EnemyManager)
	{
		TSet<FString> DeadEnemyID = SaveManager->GetEnemySpawnSave();
		EnemyManager->SetDeadEnemyID(DeadEnemyID);
	}
	
	// 로딩 레벨로 이동 후, 1초 후 package load 
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPFGameInstance::HandleStartLoading, 1.0f, false);
}

void UPFGameInstance::ClearGame()
{
	if (SaveManager)
	{
		SaveManager->ClearGame();
	}
}

void UPFGameInstance::PlayBGM(UWorld* World)
{
	FString FullPath = World->GetOutermost()->GetName(); // /Game/Game/Level/UEDPIE_0_TestMap
	FString CleanPath = GetLevelCleanPath(FullPath);
	
	FPrimaryAssetId PrimaryAssetId = FPrimaryAssetId("Map", *CleanPath);
	
	if (LevelNameMap.Contains(PrimaryAssetId))
	{
		if (SoundManager)
		{
			SoundManager->PlayBGM(LevelNameMap[PrimaryAssetId], World);
		}
	}
}

void UPFGameInstance::HandlePostWorldInitialization(UWorld* World, const UWorld::InitializationValues IVS)
{
	if (bMonsterSpawned) return;
	if (!World->IsGameWorld()) return;
	
	FString FullPath = World->GetOutermost()->GetName();
	FString CleanPath = GetLevelCleanPath(FullPath);

	FPrimaryAssetId PrimaryAssetId = FPrimaryAssetId("Map", *CleanPath);
	
	if (LevelNameMap.Contains(PrimaryAssetId))
	{
		if (EnemyManager)
		{
			bMonsterSpawned = true;
			EnemyManager->SpawnMonster(World, *LevelNameMap[PrimaryAssetId]);
		}
	}
}

FString UPFGameInstance::GetLevelCleanPath(FString Path) const
{
	// 짧은 이름만 추출
	FString ShortName = FPackageName::GetShortName(Path);  // UEDPIE_0_TestMap

	// "UEDPIE_0_" 제거
	FString CleanShortName = ShortName;
	CleanShortName.RemoveFromStart(TEXT("UEDPIE_0_"));
	
	// 경로에서 앞부분 정리
	FString CleanPath = Path.Replace(*ShortName, *CleanShortName);

	return CleanPath;
}

void UPFGameInstance::HandleStartLoading()
{
	LoadingScreenWidget = CreateWidget<UUserWidget>(this , LoadingScreenWidgetClass);
	if (LoadingScreenWidget)
	{
        UGameViewportClient* Viewport = GetWorld()->GetGameViewport();
        if (Viewport)
        {
            Viewport->AddViewportWidgetContent(LoadingScreenWidget->TakeWidget(), 100);
        }
	}
	
	FString CleanPath = GetLevelCleanPath(TargetLevelPath);
	FPrimaryAssetId PrimaryAssetId = FPrimaryAssetId("Map", *CleanPath);
	
	if (PrimaryAssetId.IsValid() && PFAssetManager)
	{
		CurrentLoadingLevelPath = CleanPath;
		PFAssetManager->RegisterLoadAssetByID(PrimaryAssetId);
		
		FSimpleDelegate SimpleDelegate;
		SimpleDelegate.BindUObject(this, &ThisClass::HandleLoadEnd);
		
		PFAssetManager->StartLoadAsset(SimpleDelegate);
	}
	
	/*// Async Load
	UAssetManager& Manager = UAssetManager::Get();
	FLoadPackageAsyncDelegate LoadedDelegate;
	LoadedDelegate.BindLambda([this](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
	{
		UE_LOG(LogTemp, Warning, TEXT("Package Loaded: %s"), *PackageName.ToString());
		bPreOpenLevel = false;
		// 실제 맵으로 전환
		FString MapName = FPackageName::GetShortName(TargetLevelPath);
		
		FString CleanPath;
		MapName.Split(TEXT("."), nullptr, &CleanPath);  // Game/Game/Level/UEDPIE_0_TestMap

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle,
		[this, CleanPath, MapName]()
		{
			bMonsterSpawned = false;
			if (CleanPath.IsEmpty())
			{
				UGameplayStatics::OpenLevel(GetWorld(), FName(*MapName));	
			}
			else
			{
				UGameplayStatics::OpenLevel(GetWorld(), FName(*CleanPath));
			}
			
			LoadingScreenWidget->RemoveFromParent();
		},
		2.f, false);
	});
	
	// 비동기 로딩 시작
	LoadPackageAsync(TargetLevelPath, LoadedDelegate, 0, PKG_ContainsMap);*/
}

void UPFGameInstance::HandleLoadEnd()
{
	bMonsterSpawned = false;
	bPreOpenLevel = false;
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,
	[this]()
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentLoadingLevelPath));
	
		UGameViewportClient* Viewport = GetWorld()->GetGameViewport();
		if (Viewport)
		{
			Viewport->RemoveViewportWidgetContent(LoadingScreenWidget->TakeWidget());
			LoadingScreenWidget = nullptr;
		}
	},
	2.f, false);
}
