#pragma once
#include "ICombatState.h"

class PROJECTF_API StaggerState : public ICombatState
{
public:
	virtual ~StaggerState() override = default;
	
	virtual void Enter(UCombatStateComponent* CombatComponent) override;
	virtual void Tick(UCombatStateComponent* CombatComponent, float DeltaTime) override;
	virtual void Exit(UCombatStateComponent* CombatComponent) override;
	virtual bool CanChangeState(ECombatState CharacterStateType) override;
	
	virtual ECombatState GetStateType() override;

	static TUniquePtr<StaggerState> Create() { return MakeUnique<StaggerState>(); }
};
