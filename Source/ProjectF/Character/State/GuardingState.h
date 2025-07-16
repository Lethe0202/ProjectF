#pragma once

#include "ICombatState.h"

class PROJECTF_API GuardingState : public ICombatState
{
public:
	virtual ~GuardingState() override = default;
	
	virtual void Enter(UCombatStateComponent* CombatComponent) override;
	virtual void Tick(UCombatStateComponent* CombatComponent, float DeltaTime) override;
	virtual void Exit(UCombatStateComponent* CombatComponent) override;
	virtual bool CanChangeState(ECombatState CharacterStateType) override;
	
	virtual ECombatState GetStateType() override;
	
	static TUniquePtr<GuardingState> Create() { return MakeUnique<GuardingState>(); }
};
