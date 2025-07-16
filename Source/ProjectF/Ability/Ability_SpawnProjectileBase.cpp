// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_SpawnProjectileBase.h"

#include "GameFramework/Character.h"
#include "ProjectF/Common/CombatTypes.h"
#include "Projectile/ProjectileBase.h"

class UEffectType;

void UAbility_SpawnProjectileBase::AbilityEffect(int Index)
{
	Super::AbilityEffect(Index);

	if (Index == 0)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Owner->GetActorLocation());
		SpawnTransform.SetRotation(Owner->GetActorRotation().Quaternion());
		
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(Owner))
		{
			FVector SocketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(SpawnSocketName);
			if (!SocketLocation.IsZero())
			{
				SpawnTransform.SetLocation(SocketLocation); 
			}
		}
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Owner.Get();
		
		AProjectileBase* ProjectileBase = Owner->GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnTransform, SpawnParams);
		ProjectileBase->SetInstigator(Owner.Get()->GetInstigator());
		ProjectileBase->OnProjectileEffect.AddDynamic(this, &UAbility_SpawnProjectileBase::OnHit);
	}
}

void UAbility_SpawnProjectileBase::OnHit_Implementation(AActor* SelfActor, AActor* OtherActor, const FHitResult& Hit)
{
	if (!AbilityDataAsset->EffectContainer.IsValidIndex(0)) return;
	
	const TArray<TObjectPtr<UEffectType>>& Effects = AbilityDataAsset->EffectContainer[0].Effect;
	for (int i = 0; i < Effects.Num(); ++i)
	{
		FTransform EffectTransform;
		EffectTransform.SetLocation(Hit.ImpactPoint);
		
		Effects[i]->ApplyEffect(OtherActor, SelfActor, EffectTransform, AbilityDataAsset->EffectContainer[0].bStrongEffect);
		
		SelfActor->Destroy();
	}
}
