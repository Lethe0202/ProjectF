#include "DodgingState.h"

void DodgingState::Enter(UCombatStateComponent* CombatComponent)
{
}

void DodgingState::Tick(UCombatStateComponent* CombatComponent, float DeltaTime)
{
}

void DodgingState::Exit(UCombatStateComponent* CombatComponent)
{
}

ECombatState DodgingState::GetStateType()
{
	return ECombatState::Dodging;
}

bool DodgingState::CanChangeState(ECombatState CharacterStateType)
{
	bool bCanChangeState = false;
	
	bCanChangeState |= CharacterStateType == ECombatState::Idle;
	bCanChangeState |= CharacterStateType == ECombatState::Stagger;
	bCanChangeState |= CharacterStateType == ECombatState::Launch;
	bCanChangeState |= CharacterStateType == ECombatState::TakeDown;
	bCanChangeState |= CharacterStateType == ECombatState::Dead;
	//bCanChangeState |= CharacterStateType == ECombatState::Dodging;
	
	return bCanChangeState;
}
