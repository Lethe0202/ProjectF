#include "ExecutionState.h"

void ExecutionState::Enter(UCombatStateComponent* CombatComponent)
{
}

void ExecutionState::Tick(UCombatStateComponent* CombatComponent, float DeltaTime)
{
}

void ExecutionState::Exit(UCombatStateComponent* CombatComponent)
{
}

ECombatState ExecutionState::GetStateType()
{
	return ECombatState::Execution;
}

bool ExecutionState::CanChangeState(ECombatState CharacterStateType)
{
	bool bCanChangeState = false;
	
	bCanChangeState |= CharacterStateType == ECombatState::Idle;
	
	return bCanChangeState;
}