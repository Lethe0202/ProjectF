#include "EffectTypes.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

#include "ProjectF/Character/PFCharacterBase.h"
#include "ProjectF/Character/Component/CombatStateComponent.h"
#include "ProjectF/Character/State/ICombatState.h"
#include "ProjectF/DataAsset/HitSFXDataAsset.h"
#include "ProjectF/Manager/AnimManager.h"
#include "ProjectF/Manager/PFGameInstance.h"

void UEffectType::ApplyEffect_Implementation(AActor* Target, AActor* EffectCauser, FEffectInfo EffectInfo, bool bStrongEffect) const
{
	return;
}

bool UEffectType::CanApplyEffect(AActor* Target) const
{
	ACharacter* Character = Cast<ACharacter>(Target);
	if (!Character) return false;
	
	APFCharacterBase* PFCharacterBase = Cast<APFCharacterBase>(Character);
	if (!PFCharacterBase) return false;
	
	UCombatStateComponent* TargetCombatState = PFCharacterBase->GetCombatStateComponent();
	if (!TargetCombatState) return false;

	if (TargetCombatState->GetCurrentState() == ECombatState::Dead)
	{
		return false;
	}

	return true;
}

void UEffectType_Damage::ApplyEffect_Implementation(AActor* Target, AActor* EffectCauser, FEffectInfo EffectInfo, bool bStrongEffect) const
{
	if (!CanApplyEffect(Target))
	{
		return;
	}
	
	FVector HitFromDirection = EffectInfo.EffectTransform.GetLocation();
	
	TSubclassOf<UDamageType> DamageType = nullptr;
	if (bStrongEffect)
	{
		DamageType = UStrongDamageType::StaticClass();
	}
	
	AController* Instigator = EffectCauser->GetInstigatorController();
	UGameplayStatics::ApplyPointDamage(Target, Damage, HitFromDirection, EffectInfo.Hit, Instigator, EffectCauser, DamageType);
}

void UEffectType_HitEffect::ApplyEffect_Implementation(AActor* Target, AActor* EffectCauser, FEffectInfo EffectInfo, bool bStrongEffect) const
{
	if (ParticleSystem)
	{
		UParticleSystemComponent* ParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(Target->GetWorld(), ParticleSystem, EffectInfo.EffectTransform.GetLocation());
		float WorldTimeDelation = 1.f / UGameplayStatics::GetGlobalTimeDilation(Target->GetWorld());
		ParticleSystemComponent->CustomTimeDilation = WorldTimeDelation;
	}

	ACharacter* Character = Cast<ACharacter>(Target);
	if (!Character) return;
	
	APFCharacterBase* PFCharacterBase = Cast<APFCharacterBase>(Character);
	if (!PFCharacterBase) return;
	
	if (PFCharacterBase->IsGuardCounterSuccess()) return;
	if (PFCharacterBase->GetCombatStateComponent()->GetCurrentState() == ECombatState::Guard) return;
	
	if (HitSFXDataAsset && EffectInfo.Hit.PhysMaterial.IsValid() && HitSFXDataAsset->SurfaceSoundMap.Contains(EffectInfo.Hit.PhysMaterial->SurfaceType))
	{
		USoundBase* SoundCue = HitSFXDataAsset->SurfaceSoundMap[EffectInfo.Hit.PhysMaterial->SurfaceType];
		if (SoundCue)
		{
			UGameplayStatics::PlaySoundAtLocation(Target->GetWorld(), SoundCue, EffectInfo.EffectTransform.GetLocation());
		}
	}
}

void UEffectType_BeamEffect::ApplyEffect_Implementation(AActor* Target, AActor* EffectCauser, FEffectInfo EffectInfo, bool bStrongEffect) const
{
	if (!CanApplyEffect(Target))
	{
		return;
	}
	
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
			ParticleSystemComponent->SetVectorParameter("BeamEnd", EffectInfo.EffectTransform.GetLocation());
			ParticleSystemComponent->Activate(false);
		}
	}
}

void UEffectType_Stagger::ApplyEffect_Implementation(AActor* Target, AActor* EffectCauser, FEffectInfo EffectInfo, bool bStrongEffect) const
{
	if (!CanApplyEffect(Target))
	{
		return;
	}
	
	ACharacter* Character = Cast<ACharacter>(Target);
	if (!Character) return;
	
	APFCharacterBase* PFCharacterBase = Cast<APFCharacterBase>(Character);
	if (!PFCharacterBase) return;
	
	if (PFCharacterBase->IsGuardCounterSuccess()) return;
	if (PFCharacterBase->GetCombatStateComponent()->GetIsCountering()) return;
	if (PFCharacterBase->GetCombatStateComponent()->GetInvincible()) return;
	if (PFCharacterBase->GetCombatStateComponent()->GetSuperArmor()) return;
		
	bool bCanStagger = PFCharacterBase->CanChangeState(ECombatState::Stagger);
	if (!bCanStagger) return;

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
	
	FVector ForwardVector = Target->GetActorForwardVector();
	
	float DotProduct = FVector::DotProduct(ForwardVector, DirectionVector * -1);
	float Angle = FMath::Acos(DotProduct) * (180.f / PI);
	
	// 방향 판별용 (좌우)
	float CrossZ = FVector::CrossProduct(ForwardVector, DirectionVector * -1).Z;
	
	if (CrossZ < 0)
	{
		Angle = -Angle;
	}
	
	ECharacterDirection CharacterDirection;
	
	if (-46.f <= Angle && Angle <= 46.f) // 정면
	{
		CharacterDirection = ECharacterDirection::Forward;
	}
	else if (46 < Angle && Angle < 136) // 오른쪽
	{
		CharacterDirection = ECharacterDirection::Right;
	}
	else if (-136 < Angle && Angle < -46) // 왼쪽
	{
		CharacterDirection = ECharacterDirection::Left;
	}
	else // 뒤
	{
		CharacterDirection = ECharacterDirection::Backward;
	}
	
	if (UAnimMontage* Montage = PFGameInstance->GetAnimManager()->GetHitDirection(PFCharacterBase->GetID(), CharacterDirection))
	{
		if (PFCharacterBase->GetMesh() && PFCharacterBase->GetMesh()->GetAnimInstance())
		{
			PFCharacterBase->GetMesh()->GetAnimInstance()->Montage_Play(Montage);	
		}
	}
}

void UEffectType_Launch::ApplyEffect_Implementation(AActor* Target, AActor* EffectCauser, FEffectInfo EffectInfo, bool bStrongEffect) const
{
	if (!CanApplyEffect(Target))
	{
		return;
	}
	
	if (bChangestate)
	{
		if (bStrongEffect)
		{
			StrongEffect(Target, EffectCauser, EffectInfo.EffectTransform);
		}
		else
		{
			NormalEffect(Target, EffectCauser, EffectInfo.EffectTransform);
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
			
	bool bCanLaunch = PFCharacterBase->CanChangeState(ECombatState::Launch);
	if (bCanLaunch)
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
				PFCharacterBase->ChangeState(ECombatState::Launch);
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




