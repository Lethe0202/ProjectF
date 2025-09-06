#include "LaunchState.h"

void LaunchState::Enter(UCombatStateComponent* CombatComponent)
{
}

void LaunchState::Tick(UCombatStateComponent* CombatComponent, float DeltaTime)
{
}

void LaunchState::Exit(UCombatStateComponent* CombatComponent)
{
	/*ActionComponent->GetOwner();
	if (AController* Controller = OwnerActor->GetInstigatorController())
	{
		Controller->StopMovement();
	}*/
}

ECombatState LaunchState::GetStateType()
{
	return ECombatState::Launch;
}

bool LaunchState::CanChangeState(ECombatState CharacterStateType)
{
	bool bCanChangeState = false;
	bCanChangeState |= CharacterStateType == ECombatState::Idle;
	//bCanChangeState |= CharacterStateType == ECombatState::Attacking;
	//bCanChangeState |= CharacterStateType == ECombatState::Stagger;
	bCanChangeState |= CharacterStateType == ECombatState::Launch;
	bCanChangeState |= CharacterStateType == ECombatState::Execution;
	bCanChangeState |= CharacterStateType == ECombatState::TakeDown;
	bCanChangeState |= CharacterStateType == ECombatState::Dead;

	return bCanChangeState;
}
