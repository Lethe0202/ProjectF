#include "GuardingState.h"

void GuardingState::Enter(UCombatStateComponent* CombatComponent)
{
}

void GuardingState::Tick(UCombatStateComponent* CombatComponent, float DeltaTime)
{
}

void GuardingState::Exit(UCombatStateComponent* CombatComponent)
{
}

bool GuardingState::CanChangeState(ECombatState CharacterStateType)
{
	bool bCanChangeState = false;
	
	bCanChangeState |= CharacterStateType == ECombatState::Idle;
	
	return bCanChangeState;
}

ECombatState GuardingState::GetStateType()
{
	return ECombatState::Guard;
}