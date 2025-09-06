// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_SpawnBezierProjectileBase.h"

#include "GameFramework/Character.h"
#include "Projectile/BezierProjectileBase.h"
#include "Projectile/ProjectileBase.h"

AProjectileBase* UAbility_SpawnBezierProjectileBase::SpawnProjectile()
{
	if (!ProjectileClass) return nullptr;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(Owner->GetActorLocation());
	SpawnTransform.SetRotation(Owner->GetActorRotation().Quaternion());
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner.Get();
	
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(Owner))
	{
		FVector SocketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(SpawnSocketName);
		if (!SocketLocation.IsZero())
		{
			SpawnTransform.SetLocation(SocketLocation);
		}
	}
	
	ABezierProjectileBase* BezierProjectile = Owner->GetWorld()->SpawnActor<ABezierProjectileBase>(ProjectileClass, SpawnTransform, SpawnParams);
	if (!BezierProjectile) return nullptr;

	if (EffectedActors.IsValidIndex(0))
	{
		BezierProjectile->InitBezierProjectile(EffectedActors[0]->GetTransform(), Duration);
	}
	
	return BezierProjectile;
}
