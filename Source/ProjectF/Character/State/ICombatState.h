#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	Idle = 0,
	Attacking,
	Stagger,
	Launch,
	TakeDown,
	Dodging,
	Parkour,
	Guard,
	Stun,
	Execution,
	Dead,
};

class UCombatStateComponent;

class PROJECTF_API ICombatState
{
public:
	virtual ~ICombatState() = default;
	
	virtual void Enter(UCombatStateComponent* CombatComponent) PURE_VIRTUAL(ICombatState::Enter, );
	virtual void Tick(UCombatStateComponent* CombatComponent, float DeltaTime) PURE_VIRTUAL(ICombatState::Tick, );
	virtual void Exit(UCombatStateComponent* CombatComponent) PURE_VIRTUAL(ICombatState::Exit, );
	virtual bool CanChangeState(ECombatState CharacterStateType) PURE_VIRTUAL(ICombatState::CanChangeState, return false;);
    
	virtual bool CanMove() PURE_VIRTUAL(ICombatState::CanChangeState, return false;);
	virtual ECombatState GetStateType() PURE_VIRTUAL(ICombatState::CanChangeState, return {};);
	
	static TUniquePtr<ICombatState> Create() { return MakeUnique<ICombatState>(); }
};
