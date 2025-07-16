#include "IdleState.h"

#include "ProjectF/Character/Component/CombatStateComponent.h"

void IdleState::Enter(UCombatStateComponent* CombatComponent)
{
}

void IdleState::Tick(UCombatStateComponent* CombatComponent, float DeltaTime)
{
}

void IdleState::Exit(UCombatStateComponent* CombatComponent)
{
	AActor* Owner = CombatComponent->GetOwner();
	
	if (AController* Controller = Owner->GetInstigatorController())
	{
		Controller->StopMovement();
	}
}

ECombatState IdleState::GetStateType()
{
	return ECombatState::Idle;
}

bool IdleState::CanChangeState(ECombatState CharacterStateType)
{
	if (CharacterStateType != ECombatState::Idle)
	{
		return true;
	}
	
	return false;
}
