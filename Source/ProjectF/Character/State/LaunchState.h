#pragma once
#include "ICombatState.h"

class PROJECTF_API LaunchState : public ICombatState
{
public:
	virtual ~LaunchState() override = default;
	
	virtual void Enter(UCombatStateComponent* CombatComponent) override;
	virtual void Tick(UCombatStateComponent* CombatComponent, float DeltaTime) override;
	virtual void Exit(UCombatStateComponent* CombatComponent) override;
	virtual bool CanChangeState(ECombatState CharacterStateType) override;
    
	//virtual bool CanMove() override;
	virtual ECombatState GetStateType() override;

	static TUniquePtr<LaunchState> Create() { return MakeUnique<LaunchState>(); }
};
