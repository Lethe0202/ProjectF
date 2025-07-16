// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TryActivateAbility.generated.h"

class UAbilityBase;
class APFCharacterBase;


USTRUCT()
struct FTryActivateAbilityMemory
{
	GENERATED_BODY()
	
	float ElapsedTime = 0.f;

	UPROPERTY()
	TWeakObjectPtr<APFCharacterBase> PFCharacterBase;
};

/**
 * 
 */
UCLASS()
class PROJECTF_API UBTTask_TryActivateAbility : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual uint16 GetInstanceMemorySize() const;
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAbilityBase> AbilityClass;
};
