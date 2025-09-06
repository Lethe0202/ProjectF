#include "StaggerState.h"

void StaggerState::Enter(UCombatStateComponent* CombatComponent)
{
}

void StaggerState::Tick(UCombatStateComponent* CombatComponent, float DeltaTime)
{
}

void StaggerState::Exit(UCombatStateComponent* CombatComponent)
{
}

bool StaggerState::CanChangeState(ECombatState CharacterStateType)
{
	bool bCanChangeState = false;

	bCanChangeState |= CharacterStateType == ECombatState::Idle;
	bCanChangeState |= CharacterStateType == ECombatState::Stun;
	bCanChangeState |= CharacterStateType == ECombatState::Execution;
	bCanChangeState |= CharacterStateType == ECombatState::Stagger;
	bCanChangeState |= CharacterStateType == ECombatState::Launch;
	bCanChangeState |= CharacterStateType == ECombatState::TakeDown;
	bCanChangeState |= CharacterStateType == ECombatState::Dead;
	
	return bCanChangeState;
}

ECombatState StaggerState::GetStateType()
{
	return ECombatState::Stagger;
}
