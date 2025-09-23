#pragma once

#include "Engine/DataTable.h"
#include "EnemySpawnTypes.generated.h"

class APFAICharacter;

USTRUCT()
struct FEnemySpawnInfo
{
	GENERATED_BODY()

	FEnemySpawnInfo()
	: Location(0.0f)
	, Rotation(0.0f)
	{
	}
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<APFAICharacter> EnemyClass;

	UPROPERTY(EditAnywhere)
	FVector Location;

	UPROPERTY(EditAnywhere)
	FRotator Rotation;
};

USTRUCT(BlueprintType)
struct FEnemySpawnTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FEnemySpawnInfo> EnemySpawnInfos;
};