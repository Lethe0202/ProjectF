// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TryActivateAbility.h"

#include "AIController.h"
#include "ProjectF/Character/PFCharacterBase.h"
#include "ProjectF/Character/Component/AbilityComponent.h"
#include "ProjectF/Character/State/ICombatState.h"

EBTNodeResult::Type UBTTask_TryActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bNotifyTick = true;
	
	FTryActivateAbilityMemory* Memory = (FTryActivateAbilityMemory*)NodeMemory;
	Memory->ElapsedTime = 0.f;  // 초기화
	
	Memory->PFCharacterBase = Cast<APFCharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Memory->PFCharacterBase.IsValid())
	{
		return EBTNodeResult::Failed;
	}
	
	if (UAbilityComponent* AbilityComponent= Memory->PFCharacterBase->GetComponentByClass<UAbilityComponent>())
	{
		bool bChangedState = Memory->PFCharacterBase->CanChangeState(ECombatState::Attacking);
		if (!bChangedState) return EBTNodeResult::Failed;

		bool bAbilitySuccess = AbilityComponent->ActivateAbility(AbilityClass);
		if (bAbilitySuccess)
		{
			Memory->PFCharacterBase->ChangeState(ECombatState::Attacking);
			return EBTNodeResult::InProgress;
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}
	
	return EBTNodeResult::Failed;
}

uint16 UBTTask_TryActivateAbility::GetInstanceMemorySize() const
{
	return sizeof(FTryActivateAbilityMemory);
}

void UBTTask_TryActivateAbility::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	FTryActivateAbilityMemory* Memory = (FTryActivateAbilityMemory*)NodeMemory;
	Memory->ElapsedTime += DeltaSeconds;
	
	if (Memory->ElapsedTime >= 0.25f)
	{
		if (Memory->PFCharacterBase.IsValid())
		{
			if (Memory->PFCharacterBase->GetCurrentState() != ECombatState::Attacking)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}
}
