#include "AttackingState.h"

void AttackingState::Enter(UCombatStateComponent* CombatComponent)
{
}

void AttackingState::Tick(UCombatStateComponent* CombatComponent, float DeltaTime)
{
}

void AttackingState::Exit(UCombatStateComponent* CombatComponent)
{
}

bool AttackingState::CanChangeState(ECombatState CharacterStateType)
{
	bool bCanChangeState = false;
	
	bCanChangeState |= CharacterStateType == ECombatState::Idle;
	bCanChangeState |= CharacterStateType == ECombatState::Stagger;
	bCanChangeState |= CharacterStateType == ECombatState::Launch;
	bCanChangeState |= CharacterStateType == ECombatState::TakeDown;
	bCanChangeState |= CharacterStateType == ECombatState::Stun;
	bCanChangeState |= CharacterStateType == ECombatState::Dead;
	
	return bCanChangeState;
}

ECombatState AttackingState::GetStateType()
{
	return ECombatState::Attacking;
}
