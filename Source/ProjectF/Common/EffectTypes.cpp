#include "EffectTypes.h"

#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

#include "ProjectF/Character/PFCharacterBase.h"
#include "ProjectF/Character/Component/CombatStateComponent.h"
#include "ProjectF/Character/Component/StaminaComponent.h"
#include "ProjectF/Character/State/ICombatState.h"
#include "ProjectF/Manager/AnimManager.h"
#include "ProjectF/Manager/PFGameInstance.h"

void UEffectType::ApplyEffect(AActor* Target, AActor* EffectCauser, FTransform EffectTransform, bool bStrongEffect) const
{
	return;
}

void UEffectType_Damage::ApplyEffect(AActor* Target, AActor* EffectCauser, FTransform EffectTransform, bool bStrongEffect) const
{
	if (EffectCauser)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Damage"));	
	}
	
	FHitResult Hit;
	FVector HitFromDirection;
	
	TSubclassOf<UDamageType> DamageType = nullptr;
	if (bStrongEffect)
	{
		DamageType = UStrongDamageType::StaticClass();
	}
	AController* Instigator = EffectCauser->GetInstigatorController();
	UGameplayStatics::ApplyPointDamage(Target, Damage, HitFromDirection, Hit, Instigator, EffectCauser, DamageType);
}

void UEffectType_Stamina::ApplyEffect(AActor* Target, AActor* EffectCauser, FTransform EffectTransform, bool bStrongEffect) const
{
	Super::ApplyEffect(Target, EffectCauser, EffectTransform, bStrongEffect);

	APFCharacterBase* EffectTargetCharacter = Cast<APFCharacterBase>(Target);
	if (!EffectTargetCharacter) return;

	//EffectTargetCharacter->ApplyStamina(Stamina);
}

void UEffectType_HitEffect::ApplyEffect(AActor* Target, AActor* EffectCauser, FTransform EffectTransform, bool bStrongEffect) const
{
	if (ParticleSystem)
	{
		UParticleSystemComponent* ParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(Target->GetWorld(), ParticleSystem, EffectTransform.GetLocation());
		float WorldTimeDelation = 1.f / UGameplayStatics::GetGlobalTimeDilation(Target->GetWorld());
		ParticleSystemComponent->CustomTimeDilation = WorldTimeDelation;
	}
}

void UEffectType_BeamEffect::ApplyEffect(AActor* Target, AActor* EffectCauser, FTransform EffectTransform, bool bStrongEffect) const
{
	if (!ParticleSystem) return;
	
	if (ACharacter* EffectCauserCharacter = Cast<ACharacter>(EffectCauser))
	{
		FVector StartSocketLocation = EffectCauserCharacter->GetMesh()->GetSocketLocation(SocketName);
		if (!StartSocketLocation.IsNearlyZero())
		{
			UParticleSystemComponent* ParticleSystemComponent =
				UGameplayStatics::SpawnEmitterAtLocation(Target->GetWorld(), ParticleSystem, StartSocketLocation,
					FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, EPSCPoolMethod::AutoRelease, false);
			
			float WorldTimeDelation = 1.f / UGameplayStatics::GetGlobalTimeDilation(Target->GetWorld());
			ParticleSystemComponent->CustomTimeDilation = WorldTimeDelation;
			ParticleSystemComponent->SetVectorParameter("BeamEnd", EffectTransform.GetLocation());
			ParticleSystemComponent->Activate(false);
		}
	}
}

void UEffectType_Stagger::ApplyEffect(AActor* Target, AActor* EffectCauser, FTransform EffectTransform, bool bStrongEffect) const
{
	ACharacter* Character = Cast<ACharacter>(Target);
	if (!Character) return;
	
	APFCharacterBase* PFCharacterBase = Cast<APFCharacterBase>(Character);
	if (!PFCharacterBase) return;
	
	if (PFCharacterBase->IsGuardCounterSuccess()) return;
	if (PFCharacterBase->GetCombatStateComponent()->GetIsCountering()) return;
	if (PFCharacterBase->GetCombatStateComponent()->GetInvincible()) return;
	if (PFCharacterBase->GetCombatStateComponent()->GetSuperArmor()) return;
		
	bool bLaunch = PFCharacterBase->ChangeState(ECombatState::Stagger);
	if (!bLaunch) return;

	FVector DirectionVector = (Target->GetActorLocation() - EffectCauser->GetActorLocation()).GetSafeNormal();
	FVector NewLocation = DirectionVector * Power;
	NewLocation.Z = 0;
			
	bool bZOverride = false;
	if (Power.Z > 0.f)
	{
		bZOverride = true;
	}
			
	PFCharacterBase->LaunchCharacter(NewLocation, true, bZOverride);
			
	UPFGameInstance* PFGameInstance = Cast<UPFGameInstance>(Character->GetGameInstance());
	if (!PFGameInstance) return;
	if (!PFGameInstance->GetAnimManager()) return;
			
	if (UAnimMontage* Montage = PFGameInstance->GetAnimManager()->GetHitDirection(PFCharacterBase->GetID(), ECharacterDirection::Forward))
	{
		if (PFCharacterBase->GetMesh() && PFCharacterBase->GetMesh()->GetAnimInstance())
		{
			PFCharacterBase->GetMesh()->GetAnimInstance()->Montage_Play(Montage);	
		}
	}
}

void UEffectType_Launch::ApplyEffect(AActor* Target, AActor* EffectCauser, FTransform EffectTransform, bool bStrongEffect) const
{
	if (bChangestate)
	{
		if (bStrongEffect)
		{
			StrongEffect(Target, EffectCauser, EffectTransform);
		}
		else
		{
			NormalEffect(Target, EffectCauser, EffectTransform);
		}
	}
	else // 단순 Launch (공중 공격을 위한)
	{
		ACharacter* Character = Cast<ACharacter>(Target);
		if (!Character) return;

		APFCharacterBase* PFCharacterBase = Cast<APFCharacterBase>(Character);
		if (!PFCharacterBase) return;
		
		PFCharacterBase->GetCharacterMovement()->GravityScale = 1.5f;
		PFCharacterBase->GetCharacterMovement()->Velocity = FVector::ZeroVector;
		PFCharacterBase->LaunchCharacter(Power, true, true);
	}
}

void UEffectType_Launch::StrongEffect(AActor* Target, AActor* EffectCauser, FTransform EffectTransform) const
{
	ACharacter* Character = Cast<ACharacter>(Target);
	if (!Character) return;

	APFCharacterBase* PFCharacterBase = Cast<APFCharacterBase>(Character);
	if (!PFCharacterBase) return;
	
	if (PFCharacterBase->GetCombatStateComponent()->GetIsCountering()) return;
			
	bool bLaunch = PFCharacterBase->ChangeState(ECombatState::Launch);
	if (bLaunch)
	{
		FVector DirectionVector = (Target->GetActorLocation() - EffectCauser->GetActorLocation()).GetSafeNormal();
		FVector NewLocation = DirectionVector * Power;
		NewLocation.Z = Power.Z;  // Z 유지
				
		PFCharacterBase->GetMovementComponent()->Velocity = FVector::ZeroVector;
		PFCharacterBase->LaunchCharacter(NewLocation, true, true);
				
		UPFGameInstance* PFGameInstance = Cast<UPFGameInstance>(Character->GetGameInstance());
		if (!PFGameInstance) return;
		if (!PFGameInstance->GetAnimManager()) return;
				
		if (UAnimMontage* Montage = PFGameInstance->GetAnimManager()->GetHitInAir(PFCharacterBase->GetID()))
		{
			if (PFCharacterBase->GetMesh() && PFCharacterBase->GetMesh()->GetAnimInstance())
			{
				PFCharacterBase->GetMesh()->GetAnimInstance()->Montage_Play(Montage);	
			}
		}
	}
}

void UEffectType_Launch::NormalEffect(AActor* Target, AActor* EffectCauser, FTransform EffectTransform) const
{
	ACharacter* Character = Cast<ACharacter>(Target);
	if (!Character) return;

	APFCharacterBase* PFCharacterBase = Cast<APFCharacterBase>(Character);
	if (!PFCharacterBase) return;
	
	if (PFCharacterBase->IsGuardCounterSuccess()) return;
	if (PFCharacterBase->GetCombatStateComponent()->GetIsCountering()) return;
	if (PFCharacterBase->GetCombatStateComponent()->GetInvincible()) return;
	if (PFCharacterBase->GetCombatStateComponent()->GetSuperArmor()) return;
			
	bool bLaunch = PFCharacterBase->ChangeState(ECombatState::Launch);
	if (bLaunch)
	{
		PFCharacterBase->GetMovementComponent()->Velocity = FVector::ZeroVector;
		PFCharacterBase->LaunchCharacter(Power, true, true);
				
		UPFGameInstance* PFGameInstance = Cast<UPFGameInstance>(Character->GetGameInstance());
		if (!PFGameInstance) return;
		if (!PFGameInstance->GetAnimManager()) return;
			
		if (UAnimMontage* Montage = PFGameInstance->GetAnimManager()->GetHitInAir(PFCharacterBase->GetID()))
		{
			if (PFCharacterBase->GetMesh() && PFCharacterBase->GetMesh()->GetAnimInstance())
			{
				PFCharacterBase->GetMesh()->GetAnimInstance()->Montage_Play(Montage);	
			}
		}
	}
}




