#include "DeadState.h"

void DeadState::Enter(UCombatStateComponent* CombatComponent)
{
}

void DeadState::Tick(UCombatStateComponent* CombatComponent, float DeltaTime)
{
}

void DeadState::Exit(UCombatStateComponent* CombatComponent)
{
}

ECombatState DeadState::GetStateType()
{
	return ECombatState::Dead;
}

bool DeadState::CanChangeState(ECombatState CharacterStateType)
{
	return true;
}
