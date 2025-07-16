#include "StunState.h"

void StunState::Enter(UCombatStateComponent* CombatComponent)
{
}

void StunState::Tick(UCombatStateComponent* CombatComponent, float DeltaTime)
{
}

void StunState::Exit(UCombatStateComponent* CombatComponent)
{
}

bool StunState::CanChangeState(ECombatState CharacterStateType)
{
	bool bCanChangeState = false;
	
	bCanChangeState |= CharacterStateType == ECombatState::Idle;
	
	return bCanChangeState;
}

ECombatState StunState::GetStateType()
{
	return ECombatState::Stun;
}