// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatStateComponent.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "ProjectF/Character/PFCharacterBase.h"
#include "ProjectF/Character/State/AttackingState.h"
#include "ProjectF/Character/State/DodgingState.h"
#include "ProjectF/Character/State/ExecutionState.h"
#include "ProjectF/Character/State/GuardingState.h"
#include "ProjectF/Character/State/IdleState.h"
#include "ProjectF/Character/State/LaunchState.h"
#include "ProjectF/Character/State/ParkourState.h"
#include "ProjectF/Character/State/StaggerState.h"
#include "ProjectF/Character/State/StunState.h"
#include "ProjectF/Character/State/TakeDownState.h"
#include "ProjectF/Manager/AnimManager.h"
#include "ProjectF/Manager/PFGameInstance.h"

class UPFGameInstance;

UCombatStateComponent::UCombatStateComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatStateComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CombatState = MakeUnique<IdleState>();

	if (GetOwner())
	{
		OwnerCharacter = Cast<APFCharacterBase>(GetOwner());
		if (OwnerCharacter)
		{
			OwnerCharacter->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UCombatStateComponent::OnMontageEnded);
		}
	}
}

void UCombatStateComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		if (CombatState->GetStateType() == ECombatState::Dodging)
		{
			ChangeState(ECombatState::Idle);
		}
	}
	
	if (!OwnerCharacter) return;
	if (OwnerCharacter->IsWeaopnAttachedHand()) return;
	
	UPFGameInstance* PFGameInstance = Cast<UPFGameInstance>(OwnerCharacter->GetGameInstance());
	if (!PFGameInstance->GetAnimManager()) return;
		
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;
	if (AnimInstance->GetCurrentActiveMontage()) return;
	
	UAnimMontage* WeaponEquipMontage = PFGameInstance->GetAnimManager()->GetWeaponEquip(OwnerCharacter->GetID());
	if (WeaponEquipMontage)
	{
		AnimInstance->Montage_Play(WeaponEquipMontage);
	}
}

void UCombatStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UCombatStateComponent::ChangeState(ECombatState newState)
{
	if (!CombatState) return false;
	if (!CombatState->CanChangeState(newState))
	{
		return false;
	}
	
	AAIController* AIController = Cast<AAIController>(Cast<APawn>(GetOwner())->GetController());
	
	switch (newState)
	{
	case ECombatState::Idle:
		ChangeToState<IdleState>();
		if (AIController && AIController->GetBrainComponent())
		{
			AIController->GetBrainComponent()->ResumeLogic("Idle");
		}
		break;
	
	case ECombatState::Attacking:
		ChangeToState<AttackingState>();
		break;
	
	case ECombatState::Stagger:
		ChangeToState<StaggerState>();
		OwnerCharacter->StopLogic("Stagger");
		break;
		
	case ECombatState::Launch:
		ChangeToState<LaunchState>();
		OwnerCharacter->StopLogic("Launch");
		break;
	
	case ECombatState::TakeDown:
		ChangeToState<TakeDownState>();
		OwnerCharacter->StopLogic("TakeDown");
		break;
	
	case ECombatState::Dodging:
        ChangeToState<DodgingState>();
		bDodging = false;
        break;

	case ECombatState::Parkour:
		ChangeToState<ParkourState>();
		break;

	case ECombatState::Guard:
		ChangeToState<GuardingState>();
		break;
		
	case ECombatState::Stun:
		ChangeToState<StunState>();
		break;
	
	case ECombatState::Execution:
		ChangeToState<ExecutionState>();
		break;
	
	case ECombatState::Dead:
		//ChangeToState<IdleState>();
		OwnerCharacter->StopLogic("Dead");
		break;
	}
	
	return true;
}

bool UCombatStateComponent::CanChangeState(ECombatState NewState)
{
	return CombatState->CanChangeState(NewState);
}

void UCombatStateComponent::ActivateCounter(AActor* Target, FDamageEvent const& DamageEvent)
{
	if (bDodging)
	{
		return;
	}
	
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		
		FTransform ParticleTransform;
		ParticleTransform.SetLocation(GetOwner()->GetActorLocation());
		
		if (UParticleSystemComponent* ParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ConuterParticleSystem, ParticleTransform))
		{
			ParticleSystemComponent->CustomTimeDilation = 0.2f;
		}
		
		if (!bDodging)
		{
			bDodging = true;
			PlayCounterEffect(Target);
		}
	}
}

template<typename NewStateType>
void UCombatStateComponent::ChangeToState()
{
	if (CombatState)
	{
		CombatState->Exit(this);
	}
	
	CombatState = MakeUnique<NewStateType>();
	if (CombatState)
	{
		CombatState->Enter(this);	
	}
}
