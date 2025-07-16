#pragma once

#include "ICombatState.h"

class UCombatStateComponent;

class PROJECTF_API IdleState : public ICombatState
{
public:
	virtual ~IdleState() override = default;
	
	virtual void Enter(UCombatStateComponent* CombatComponent) override;
	virtual void Tick(UCombatStateComponent* CombatComponent, float DeltaTime) override;
	virtual void Exit(UCombatStateComponent* CombatComponent) override;
	virtual bool CanChangeState(ECombatState CharacterStateType) override;
	
	virtual ECombatState GetStateType() override;
	
	static TUniquePtr<IdleState> Create() { return MakeUnique<IdleState>(); }
};
