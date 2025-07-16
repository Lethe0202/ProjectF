// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "ProjectF/Ability/AbilityBase.h"
#include "ProjectF/Character/PFCharacterBase.h"
#include "ProjectF/Character/Component/AbilityComponent.h"
#include "ProjectF/Character/Component/CombatStateComponent.h"
#include "ProjectF/Character/State/ICombatState.h"
#include "ProjectF/Manager/AnimManager.h"
#include "ProjectF/Manager/PFGameInstance.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));

	WeaponChargeParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WeaponChargeParticle"));
	WeaponChargeParticle->SetupAttachment(SkeletalMeshComponent);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	ShapeComponent->IgnoreActorWhenMoving(GetOwner(), true);
	ShapeComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnWeaponBeginOverlap);
	
	if (!ChargeAttackAbilityClass) return;
	
	UObject* Outer = nullptr;
	if (Owner)
	{
		Outer = Owner;	
	}
	else
	{
		Outer = this;
	}
	
	if (ChargeAttackAbilityClass)
	{
		ChargeAttackAbility = NewObject<UAbilityBase>(Outer, ChargeAttackAbilityClass);	
	}
	
	if (LaunchAttackAbilityClass)
	{
		LaunchAttackAbility = NewObject<UAbilityBase>(Outer, LaunchAttackAbilityClass);	
	}
		
	if (APFCharacterBase* PFCharacterBase = Cast<APFCharacterBase>(Owner))
	{
		if (PFCharacterBase->GetAbilityComponent())
		{
			PFCharacterBase->GetAbilityComponent()->RegisterAbility(ChargeAttackAbility);
			PFCharacterBase->GetAbilityComponent()->RegisterAbility(LaunchAttackAbility);
		}
	}
}

void AWeaponBase::OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Owner) return;
	if (!OtherActor) return;
	if (!Owner->GetInstigatorController()) return;
	if (OtherActor == Owner) return;

	IGenericTeamAgentInterface* OwnerTeamAgent = Cast<IGenericTeamAgentInterface>(Owner.Get());
	if (!OwnerTeamAgent) return;
	
	if (OwnerTeamAgent->GetTeamAttitudeTowards(*OtherActor) != ETeamAttitude::Hostile) return;
	
	APFCharacterBase* OtherCharacter = Cast<APFCharacterBase>(OtherActor);
	if (!OtherCharacter) return;

	APFCharacterBase* OwnerCharacter = Cast<APFCharacterBase>(Owner);
	if (!OwnerCharacter) return;
	
	UPFGameInstance* PFGameInstance = Cast<UPFGameInstance>(GetGameInstance());
	if (!PFGameInstance) return;
	
	FVector HitLocation;
	OtherCharacter->GetMesh()->GetClosestPointOnCollision(GetActorLocation(), HitLocation);
	
	if (!OtherCharacter->GetCombatStateComponent()->GetInvincible())
	{
		// Owner와 대상자가 공중에 있는지 체크
		FVector Start = Owner->GetActorLocation();
		FVector End = Start - FVector(0.f, 0.f, 30.f + OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());  // 30cm 아래
		
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Owner);
		
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECC_Visibility,
			Params
		);

		FVector TargetStart = OtherCharacter->GetActorLocation();
		FVector TargetEnd = TargetStart - FVector(0.f, 0.f, 30.f + OtherCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());  // 30cm 아래
		
		FHitResult HitResultTarget;
		FCollisionQueryParams ParamsTarget;
		Params.AddIgnoredActor(OtherCharacter);
		
		bool bHitTarget = GetWorld()->LineTraceSingleByChannel(
			HitResultTarget,
			TargetStart,
			TargetEnd,
			ECC_Visibility,
			ParamsTarget
		);

		// 점프 공격
		if (OtherCharacter->GetCharacterMovement()->MovementMode == MOVE_Falling && !bHitTarget)
		{
			if (JumpAttackDataAsset == nullptr) return;
			if (JumpAttackDataAsset->EffectContainer.IsValidIndex(CurrentEffectIndex) == false) return;
			
			for (const UEffectType* EffectType : JumpAttackDataAsset->EffectContainer[CurrentEffectIndex].Effect)
			{
				FTransform EffectTransform;
				
				EffectTransform.SetLocation(HitLocation);
				EffectType->ApplyEffect(OtherCharacter, Owner.Get(), EffectTransform, JumpAttackDataAsset->EffectContainer[CurrentEffectIndex].bStrongEffect);
			}
		}
		else // 지상공격
		{
			if (CurrentWeaponAttackType == EWeaponAttackType::Primary)
			{
				if (PrimaryAttackDataAsset == nullptr) return;
				if (PrimaryAttackDataAsset->EffectContainer.IsValidIndex(CurrentEffectIndex))
				{
					for (const UEffectType* EffectType : PrimaryAttackDataAsset->EffectContainer[CurrentEffectIndex].Effect)
					{
						FTransform EffectTransform;
					
						EffectTransform.SetLocation(HitLocation);
						EffectType->ApplyEffect(OtherCharacter, Owner.Get(), EffectTransform, PrimaryAttackDataAsset->EffectContainer[CurrentEffectIndex].bStrongEffect);
					}
				}
			}
			else if (CurrentWeaponAttackType == EWeaponAttackType::Strong)
			{
				if (StrongAttackDataAsset == nullptr) return;
				if (StrongAttackDataAsset->EffectContainer.IsValidIndex(CurrentEffectIndex))
				{
					for (const UEffectType* EffectType : StrongAttackDataAsset->EffectContainer[CurrentEffectIndex].Effect)
					{
						FTransform EffectTransform;
					
						EffectTransform.SetLocation(HitLocation);
						EffectType->ApplyEffect(OtherCharacter, Owner.Get(), EffectTransform, StrongAttackDataAsset->EffectContainer[CurrentEffectIndex].bStrongEffect);
					}
				}
			}
		}
		
		HitCharacters.AddUnique(OtherCharacter);
	}
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EMovementMode MovementMode = Cast<ACharacter>(Owner)->GetCharacterMovement()->MovementMode;
	if (MovementMode == MOVE_Walking)
	{
		JumpAttackCnt = 0;
		
		if (!bCanLaunchAttack)
		{
			bCanLaunchAttack = true;
		}

		if (!bCanChargeAttack)
		{
			bCanChargeAttack = true;
		}
	}

	if (ShapeComponent)
	{
		if (ShapeComponent->IsCollisionEnabled())
		{
			ShapeComponent->SetVisibility(true);	
		}
		else
		{
			ShapeComponent->SetVisibility(false);
		}
	}
}

void AWeaponBase::SetChargeEffect(bool bCharge)
{
	if (bCharge)
	{
		if (!WeaponChargeParticle->IsActive())
		{
			WeaponChargeParticle->Activate(true);
		}
	}
	else
	{
		WeaponChargeParticle->Deactivate();
	}
}

void AWeaponBase::ActivateWeaponCollision(bool bCollision, int EffectIndex)
{
	if (bCollision)
	{
		HitCharacters.Empty();
		CurrentEffectIndex = EffectIndex;
		ShapeComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		UCharacterMovementComponent* CharacterMovementComponent = Cast<ACharacter>(Owner)->GetCharacterMovement();
		
		if (CharacterMovementComponent->MovementMode == MOVE_Falling)
		{
			CharacterMovementComponent->Velocity = FVector::ZeroVector;
			CharacterMovementComponent->GravityScale = 0.1;
		}
	}
	else
	{
		HitCharacters.Empty();
		CurrentEffectIndex = -1;
		ShapeComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

UAnimMontage* AWeaponBase::GetGuardCounterMontage()
{
	UAnimMontage* ReturnMontage = nullptr;
	
	if (GuardCounterMontageArray.IsValidIndex(CurrentGuardCounterIndex))
	{
		ReturnMontage = GuardCounterMontageArray[CurrentGuardCounterIndex];
	}
	
	if (GuardCounterMontageArray.IsValidIndex(CurrentGuardCounterIndex + 1))
	{
		++CurrentGuardCounterIndex;
	}
	else
	{
		CurrentGuardCounterIndex = 0;
	}
	
	return ReturnMontage;
}

bool AWeaponBase::TryAttack()
{
	if (Owner)
	{
		if (ACharacter* Character = Cast<ACharacter>(Owner))
		{
			if (Character->GetCharacterMovement()->MovementMode == MOVE_Falling)
			{
				if (!JumpAttackDataAsset->AbilityAnim.IsValidIndex(JumpAttackCnt)) return false;
				
				Character->GetMesh()->GetAnimInstance()->Montage_Play(JumpAttackDataAsset->AbilityAnim[JumpAttackCnt]);
				CurrentWeaponAttackType = EWeaponAttackType::Primary;
				return true;
			}
			else
			{
				if (!PrimaryAttackDataAsset->AbilityAnim.IsValidIndex(AttackCnt)) return false;
				
				Character->GetMesh()->GetAnimInstance()->Montage_Play(PrimaryAttackDataAsset->AbilityAnim[AttackCnt]);
				CurrentWeaponAttackType = EWeaponAttackType::Primary;
				return true;
			}
		}
	}
	
	return false;
}

bool AWeaponBase::TryStrongAttack()
{
	if (!Owner) return false;

	ACharacter* Character = Cast<ACharacter>(Owner);
	if (!Character) return false;
	if (Character->GetCharacterMovement()->MovementMode == MOVE_Falling) return false;
	if (!StrongAttackDataAsset->AbilityAnim.IsValidIndex(StrongAttackCnt)) return false;
	
	Character->GetMesh()->GetAnimInstance()->Montage_Play(StrongAttackDataAsset->AbilityAnim[StrongAttackCnt]);
	
	bStrongAttack = true;
	CurrentWeaponAttackType = EWeaponAttackType::Strong;
	
	return true;
}

bool AWeaponBase::TryChargeAttack()
{
	if (!Owner) return false;

	ACharacter* Character = Cast<ACharacter>(Owner);
	if (!Character) return false;
	
	if (Character->GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Owner);
		
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Owner->GetActorLocation(), Owner->GetActorLocation() + FVector(0.f, 0.f, -10000.f), ECC_Visibility, Params);
		
		if (!bHit) return false;
		
		if (Owner->GetActorLocation().Z - HitResult.ImpactPoint.Z > 200.f)
		{
			if (bCanChargeAttack)
			{
				bCanChargeAttack = false;
				Character->GetMesh()->GetAnimInstance()->Montage_Play(ChargeAttackMontage);	
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (!bCanLaunchAttack) return false;
			
			bCanLaunchAttack = false;
			Character->GetMesh()->GetAnimInstance()->Montage_Play(LaunchAttackMontage);
		}
	}
	else
	{
		Character->GetMesh()->GetAnimInstance()->Montage_Play(ChargeAttackMontage);
	}

	return true;
}

void AWeaponBase::SaveAttack()
{
	if (Owner)
	{
		if (ACharacter* Character = Cast<ACharacter>(Owner))
		{
			if (Character->GetCharacterMovement()->MovementMode != MOVE_Falling)
			{
				if (bStrongAttack)
				{
					++StrongAttackCnt;
					if (StrongAttackCnt >= StrongAttackDataAsset->AbilityAnim.Num())
					{
						StrongAttackCnt = 0;
					}
				}
				else
				{
					++AttackCnt;
					if (AttackCnt >= PrimaryAttackDataAsset->AbilityAnim.Num())
					{
						AttackCnt = 0;
					}
				}
			}
			else
			{
				++JumpAttackCnt;
			}
		}
	}
}

void AWeaponBase::ResetAttack()
{
	CurrentWeaponAttackType = EWeaponAttackType::None;
	AttackCnt = 0;
	StrongAttackCnt = 0;
	bStrongAttack = false;
	SetChargeEffect(false);
	ActivateWeaponCollision(false);
	
	if (Owner)
	{
		if (ACharacter* Character = Cast<ACharacter>(Owner))
		{
			Character->GetCharacterMovement()->GravityScale = 1.5f;
		}
	}
	
	for (auto& Character : HitCharacters)
	{
		Character->GetCharacterMovement()->GravityScale = 1.5f;
	}
}

bool AWeaponBase::TryGuard()
{
	ACharacter* Character = Cast<ACharacter>(Owner);
	if (!Character) return false;

	if (GuardMontage)
	{
		Character->GetMesh()->GetAnimInstance()->Montage_Play(GuardMontage);	
		return true;
	}
	
	return false;
}

bool AWeaponBase::ReleaseGuard()
{
	ACharacter* Character = Cast<ACharacter>(Owner);
	if (!Character) return false;
	
	Character->GetMesh()->GetAnimInstance()->Montage_Stop(0.1f);
	return true;
}

