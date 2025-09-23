#include "EnemyManager.h"

#include "ProjectF/Character/PFAICharacter.h"
#include "ProjectF/System/EnemySpawnTypes.h"

void UEnemyManager::Init()
{
	
}

void UEnemyManager::SpawnMonster(UWorld* World, FName TableID)
{
	FEnemySpawnTableRow* EnemySpawnTableRow = EnemyDataTable->FindRow<FEnemySpawnTableRow>(TableID, "");
	if (!EnemySpawnTableRow) return;

	TArray<FEnemySpawnInfo> SpawnInfos = EnemySpawnTableRow->EnemySpawnInfos;
	for (const FEnemySpawnInfo& SpawnInfo : SpawnInfos)
	{
		FString ID = FString::Printf(TEXT("%s_%.0f"), *TableID.ToString(), SpawnInfo.Location.X);
		if (!DeadEnemyID.Contains(ID))
		{
			World->SpawnActor<APFAICharacter>(SpawnInfo.EnemyClass, SpawnInfo.Location, SpawnInfo.Rotation);	
		}
	}
}

void UEnemyManager::AddDeadEnemy(ACharacter* DeadCharacter, FTransform SpawnTransform)
{
	FString LevelName = DeadCharacter->GetWorld()->GetMapName();
	LevelName.RemoveFromStart(TEXT("UEDPIE_0_"));
	FString ID = FString::Printf(TEXT("%s_%.0f"), *LevelName, SpawnTransform.GetLocation().X);
	
	DeadEnemyID.Add(ID);
}

void UEnemyManager::SetDeadEnemyID(const TSet<FString>& InDeadEnemyID)
{
	this->DeadEnemyID = InDeadEnemyID;
}
