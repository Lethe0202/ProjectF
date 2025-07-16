#include "ParkourState.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectF/Character/Component/CombatStateComponent.h"

void ParkourState::Enter(UCombatStateComponent* CombatComponent)
{
	if (!CombatComponent) return;
	
	AActor* Owner = CombatComponent->GetOwner();
	if (!Owner) return;

	ACharacter* OwnerCharacter = Cast<ACharacter>(Owner);
	if (!OwnerCharacter) return;
	
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}

void ParkourState::Tick(UCombatStateComponent* CombatComponent, float DeltaTime)
{
}

void ParkourState::Exit(UCombatStateComponent* CombatComponent)
{
	if (!CombatComponent) return;
	
	AActor* Owner = CombatComponent->GetOwner();
	if (!Owner) return;

	ACharacter* OwnerCharacter = Cast<ACharacter>(Owner);
	if (!OwnerCharacter) return;

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

bool ParkourState::CanChangeState(ECombatState CharacterStateType)
{
	bool bCanChangeState = false;

	bCanChangeState |= CharacterStateType == ECombatState::Idle;
	bCanChangeState |= CharacterStateType == ECombatState::Stun;
	
	return bCanChangeState;
}

ECombatState ParkourState::GetStateType()
{
	return ECombatState::Parkour;
}