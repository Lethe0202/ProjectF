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
	
	bCanChangeState |= CharacterStateType != ECombatState::Attacking;
	bCanChangeState |= CharacterStateType != ECombatState::Stun;
	
	return bCanChangeState;
}

ECombatState StaggerState::GetStateType()
{
	return ECombatState::Stagger;
}
