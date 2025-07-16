#include "TakeDownState.h"

void TakeDownState::Enter(UCombatStateComponent* CombatComponent)
{
}

void TakeDownState::Tick(UCombatStateComponent* CombatComponent, float DeltaTime)
{
}

void TakeDownState::Exit(UCombatStateComponent* CombatComponent)
{
}

bool TakeDownState::CanChangeState(ECombatState CharacterStateType)
{
	bool bCanChangeState = false;
	bCanChangeState |= CharacterStateType == ECombatState::Idle;
	bCanChangeState |= CharacterStateType == ECombatState::Stun;
	
	return bCanChangeState;
}

ECombatState TakeDownState::GetStateType()
{
	return ECombatState::TakeDown;
}
