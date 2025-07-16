#pragma once
#include "ICombatState.h"

class  PROJECTF_API StunState : public ICombatState
{
public:
	virtual ~StunState() override = default;
	
	virtual void Enter(UCombatStateComponent* CombatComponent) override;
	virtual void Tick(UCombatStateComponent* CombatComponent, float DeltaTime) override;
	virtual void Exit(UCombatStateComponent* CombatComponent) override;
	virtual bool CanChangeState(ECombatState CharacterStateType) override;
	
	virtual ECombatState GetStateType() override;

	static TUniquePtr<StunState> Create() { return MakeUnique<StunState>(); }
};
