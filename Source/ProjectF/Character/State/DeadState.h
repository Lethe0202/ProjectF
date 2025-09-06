#pragma once

#include "ICombatState.h"
class UCombatStateComponent;

class PROJECTF_API DeadState : public ICombatState
{
public:
	virtual ~DeadState() override = default;
	
	virtual void Enter(UCombatStateComponent* CombatComponent) override;
	virtual void Tick(UCombatStateComponent* CombatComponent, float DeltaTime) override;
	virtual void Exit(UCombatStateComponent* CombatComponent) override;
	virtual bool CanChangeState(ECombatState CharacterStateType) override;
	
	virtual ECombatState GetStateType() override;
	
	static TUniquePtr<DeadState> Create() { return MakeUnique<DeadState>(); }
};