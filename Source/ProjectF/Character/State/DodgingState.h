#pragma once

#include "ICombatState.h"

class PROJECTF_API DodgingState : public ICombatState
{
public:
	virtual ~DodgingState() override = default;
	
	virtual void Enter(UCombatStateComponent* CombatComponent) override;
	virtual void Tick(UCombatStateComponent* CombatComponent, float DeltaTime) override;
	virtual void Exit(UCombatStateComponent* CombatComponent) override;
	virtual bool CanChangeState(ECombatState CharacterStateType) override;
	
	virtual ECombatState GetStateType() override;
	
	static TUniquePtr<DodgingState> Create() { return MakeUnique<DodgingState>(); }
};
