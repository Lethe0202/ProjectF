// Fill out your copyright notice in the Description page of Project Settings.


#include "PFCharacterBase.h"

#include "Component/AbilityComponent.h"
#include "Component/CombatStateComponent.h"
#include "Component/HealthComponent.h"
#include "Component/StaminaComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectF/Ability/CounterAbilityBase.h"

#include "ProjectF/Manager/AnimManager.h"
#include "ProjectF/Manager/PFGameInstance.h"
#include "ProjectF/Weapon/WeaponBase.h"

APFCharacterBase::APFCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CombatStateComponent = CreateDefaultSubobject<UCombatStateComponent>(TEXT("CombatStateComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));
}

void APFCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (StartWeaponClass)
	{
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.Owner = this;
		Weapon = GetWorld()->SpawnActor<AWeaponBase>(StartWeaponClass, ActorSpawnParameters);
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("hand_r_socket"));
	}

	CharacterInitDelegate.Broadcast();
}

void APFCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APFCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float APFCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (Damage == 0.f)
	{
		return Damage;
	}
	
	if (CombatStateComponent->GetIsCountering())
	{
		if (UAbilityBase* CurrentAbility = AbilityComponent->GetCurrentActivateAbility().Get())
		{
			if (UCounterAbilityBase* CounterAbility = Cast<UCounterAbilityBase>(CurrentAbility))
			{
				SuccessAbilityCounter();
				CounterAbility->CounterEffect();
			}
			else
			{
				CombatStateComponent->ActivateCounter(DamageCauser, DamageEvent);
			}
		}
		else
		{
			CombatStateComponent->ActivateCounter(DamageCauser, DamageEvent);
		}
	}
	else if (CombatStateComponent->GetInvincible())
	{
		if (CombatStateComponent->GetCurrentState() == ECombatState::Dodging)
		{
			if (DamageEvent.DamageTypeClass != UStrongDamageType::StaticClass())
			{
				CombatStateComponent->ActivateCounter(DamageCauser, DamageEvent);
			}
		}
	}
	else
	{
		if (IGenericTeamAgentInterface::GetTeamAttitudeTowards(*EventInstigator->GetPawn()) == ETeamAttitude::Hostile)
		{
			InterruptAction();
			ModifyDamage(Damage, DamageCauser);
		}
	}
	
	return Damage;
}

void APFCharacterBase::ModifyDamage(float DamageAmount, AActor* InInstigator)
{
	return;
}

void APFCharacterBase::SuccessGuardCounter(AActor* InInstigator)
{
	return;
}

void APFCharacterBase::SuccessGuard(float InStaminaValue)
{
	return;
}

void APFCharacterBase::SuccessAbilityCounter()
{
	return;
}

void APFCharacterBase::ResetParryInvincible()
{
	bParrySuccess = false;
}

void APFCharacterBase::SetCanAttack(bool bInCanAttack)
{
	bCanAttack = bInCanAttack;
}

float APFCharacterBase::ApplyStamina(float InStamina)
{
	return 0.f;
}

FGenericTeamId APFCharacterBase::GetGenericTeamId() const
{
	FGenericTeamId GenericTeamId(static_cast<uint8>(TeamType));
	return GenericTeamId;
}

ETeamAttitude::Type APFCharacterBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);
	if (!OtherTeamAgent)
	{
		return ETeamAttitude::Neutral;
	}

	if (OtherTeamAgent->GetGenericTeamId() == FGenericTeamId::NoTeam)
	{
		return ETeamAttitude::Neutral;
	}
	
	if (GetGenericTeamId() != OtherTeamAgent->GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Friendly;
}

ETeamAttitude::Type APFCharacterBase::GetTeamAttitudeTowardsBP(const AActor* Other) const
{
	return GetTeamAttitudeTowards(*Other);
}

void APFCharacterBase::SetControlledMovement(bool bControlledMovement)
{
	if (bControlledMovement)
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		bUseControllerRotationYaw = false;
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		bUseControllerRotationYaw = true;
	}
}

bool APFCharacterBase::TryAttack()
{
	if (Weapon->GetAttachParentSocketName() != "hand_r_socket")
	{
		return false;
	}
	
	if (bCanAttack && CanChangeState(ECombatState::Attacking))
	{
		if (Weapon->TryAttack())
		{
			ChangeState(ECombatState::Attacking);
			bCanAttack = false;
			
			return true;
		}
	}
	
	return false;
}

void APFCharacterBase::AttackHold(float HoldTime)
{
	if (ChargeHoldTime <= HoldTime)
    {
		Weapon->SetChargeEffect(true);
    }
}

bool APFCharacterBase::TryAttackHold(float HoldTime)
{
	if (ChargeHoldTime <= HoldTime)
	{
		if (CanChangeState(ECombatState::Attacking))
		{
			if (Weapon->TryChargeAttack())
			{
				ChangeState(ECombatState::Attacking);
				bCanAttack = false;

				return true;
			}
		}
	}

	return false;
}

void APFCharacterBase::SaveAttack()
{
	bCanAttack = true;
	Weapon->SaveAttack();

	ChangeState(ECombatState::Idle);
}

void APFCharacterBase::ResetAttack()
{
	bCanAttack = true;
	if (Weapon)
	{
		Weapon->ResetAttack();	
	}
	
	ChangeState(ECombatState::Idle);
	
	if (AbilityComponent)
	{
		AbilityComponent->EndAbility();
	}
}

bool APFCharacterBase::IsWeaopnAttachedHand()
{
	if (!Weapon)
	{
		return true;
	}
	
	if (Weapon->GetAttachParentSocketName() == "hand_r_socket")
	{
		return true;
	}
	else
	{
		return false;
	}
}

void APFCharacterBase::EquipWeapon(bool bEquip)
{
	if (!Weapon)
	{
		return;
	}
	
	if (bEquip)
	{
        Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("hand_r_socket"));
	}
	else
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("weapon_back_socket"));
	}
}

void APFCharacterBase::ActivateAbility(int Index)
{
	AbilityComponent->ActivateAbility(Index);
}

void APFCharacterBase::ActivateAbility(TSubclassOf<UAbilityBase> AbilityClass)
{
	AbilityComponent->ActivateAbility(AbilityClass);
}

void APFCharacterBase::InterruptAction()
{
	bCanAttack = true;
	if (Weapon)
	{
		Weapon->ResetAttack();
	}
}

void APFCharacterBase::RollCharacter(ECharacterDirection Direction)
{
	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance) return;
	
	UPFGameInstance* PFGameInstance = Cast<UPFGameInstance>(GameInstance);
	if (!PFGameInstance) return;
	
	if (PFGameInstance->GetAnimManager())
	{
		bool bCanChangeState = CanChangeState(ECombatState::Dodging);
		if (!bCanChangeState)
		{
			return;
		}
		
		UAnimMontage* RollMontage = PFGameInstance->GetAnimManager()->GetRollDirection(ID, Direction);
		if (RollMontage)
		{
			ChangeState(ECombatState::Dodging);
			
			GetMesh()->GetAnimInstance()->Montage_Play(RollMontage);
		}
	}
}

bool APFCharacterBase::ChangeState(ECombatState NewState)
{
	return CombatStateComponent->ChangeState(NewState);
}

bool APFCharacterBase::CanChangeState(ECombatState NewState)
{
	return CombatStateComponent->CanChangeState(NewState);
}

void APFCharacterBase::StopLogic(FString Reason)
{
	AbilityComponent->StopAbility();
}

void APFCharacterBase::Executed()
{
	return;
}

void APFCharacterBase::OnExecutionTrigger()
{
	return;
}

void APFCharacterBase::OnExecutionEnd()
{
	return;
}

void APFCharacterBase::OnExecutedEnd()
{
	return;
}

ECombatState APFCharacterBase::GetCurrentState() const
{
	return CombatStateComponent->GetCurrentState();	
}
