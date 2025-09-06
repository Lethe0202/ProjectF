// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EnemyManager.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECTF_API UEnemyManager : public UObject
{
	GENERATED_BODY()
	
public:
	void Init();

	UFUNCTION(BlueprintCallable)
	void SpawnMonster(UWorld* World, FName TableID);

	void AddDeadEnemy(ACharacter* DeadCharacter, FTransform SpawnTransform);
	
	void SetDeadEnemyID(const TSet<FString>& InDeadEnemyID);
	TSet<FString> GetDeadEnemyID() const { return DeadEnemyID; }
	
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> EnemyDataTable;
	
	UPROPERTY()
	TSet<FString> DeadEnemyID;
};
