#pragma once
#include "ICombatState.h"

class UCombatStateComponent;

class PROJECTF_API ParkourState : public ICombatState
{
public:
	virtual ~ParkourState() override = default;
	
	virtual void Enter(UCombatStateComponent* CombatComponent) override;
	virtual void Tick(UCombatStateComponent* CombatComponent, float DeltaTime) override;
	virtual void Exit(UCombatStateComponent* CombatComponent) override;
	virtual bool CanChangeState(ECombatState CharacterStateType) override;
	
	virtual ECombatState GetStateType() override;
	
	static TUniquePtr<ParkourState> Create() { return MakeUnique<ParkourState>(); }
};
