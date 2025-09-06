// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_SpawnProjectileBase.h"

#include "KismetTraceUtils.h"
#include "GameFramework/Character.h"
#include "ProjectF/Common/CombatTypes.h"
#include "ProjectF/Common/PFCollisionChannel.h"
#include "Projectile/ProjectileBase.h"

class UEffectType;

void UAbility_SpawnProjectileBase::AbilityEffect(int Index)
{
	Super::AbilityEffect(Index);
	
	if (Index == 0)
	{
		AProjectileBase* ProjectileBase = SpawnProjectile();
		if (ProjectileBase)
		{
			UPrimitiveComponent* RootPrim = Cast<UPrimitiveComponent>(ProjectileBase->GetRootComponent());
			/*if(RootPrim)
			{
				RootPrim->SetMobility(EComponentMobility::Movable);
				RootPrim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				RootPrim->SetCollisionResponseToAllChannels(ECR_Overlap);
			}*/
			
			ProjectileBase->SetInstigator(Owner.Get()->GetInstigator());
			ProjectileBase->OnProjectileEffect.AddDynamic(this, &UAbility_SpawnProjectileBase::OnHit);
		}
	}
}

AProjectileBase* UAbility_SpawnProjectileBase::SpawnProjectile()
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
	
	return Owner->GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnTransform, SpawnParams);
}

void UAbility_SpawnProjectileBase::OnHit_Implementation(AActor* SelfActor, AActor* OtherActor, const FHitResult& Hit)
{
	if (!AbilityDataAsset->EffectContainer.IsValidIndex(1)) return;
	
	switch (ProjectileHitType)
	{
	case EProjectileHitType::Single:
		ApplySingleEffect(SelfActor, OtherActor, Hit);
		break;
	case EProjectileHitType::Range:
		ApplyRangeEffect(SelfActor, OtherActor, Hit);
		break;
	}
}

void UAbility_SpawnProjectileBase::ApplySingleEffect(AActor* SelfActor, AActor* OtherActor, const FHitResult& Hit)
{
	const TArray<TObjectPtr<UEffectType>>& Effects = AbilityDataAsset->EffectContainer[1].Effect;
	for (int i = 0; i < Effects.Num(); ++i)
	{
		FEffectInfo EffectInfo;
		EffectInfo.Hit = Hit;
		EffectInfo.EffectTransform.SetLocation(Hit.ImpactPoint);
		
		Effects[i]->ApplyEffect(OtherActor, SelfActor, EffectInfo, AbilityDataAsset->EffectContainer[0].bStrongEffect);
			
		SelfActor->Destroy();
	}
}

void UAbility_SpawnProjectileBase::ApplyRangeEffect(AActor* SelfActor, AActor* OtherActor, const FHitResult& Hit)
{
	TArray<FHitResult> Results;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQuery;
	ObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
	TArray<FHitResult> emptyResult;
	
	//DrawDebugSphereTraceMulti(SelfActor->GetWorld(), SelfActor->GetActorLocation(), SelfActor->GetActorLocation(), ProjectileHitRadius, EDrawDebugTrace::ForDuration, false, emptyResult, FColor::Red, FColor::Green, 3.f);
	
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(MyTrace), false);
	QueryParams.AddIgnoredActor(SelfActor);

	if (SelfActor->Owner)
	{
		QueryParams.AddIgnoredActor(SelfActor->Owner);
	}
	
	bool bHit = SelfActor->GetWorld()->SweepMultiByObjectType(
		Results,
		SelfActor->GetActorLocation(),
		SelfActor->GetActorLocation(),
		FQuat::Identity,
		ObjectTypeQuery,
		FCollisionShape::MakeSphere(ProjectileHitRadius),
		QueryParams
	);
	
	if (bHit)
	{
		const TArray<TObjectPtr<UEffectType>>& Effects = AbilityDataAsset->EffectContainer[1].Effect;
		for (int i = 0; i < Effects.Num(); ++i)
		{
			for (auto& HitResult : Results)
			{
				FEffectInfo EffectInfo;
				EffectInfo.Hit = HitResult;
				EffectInfo.EffectTransform.SetLocation(HitResult.ImpactPoint);
				
				Effects[i]->ApplyEffect(OtherActor, SelfActor, EffectInfo, AbilityDataAsset->EffectContainer[1].bStrongEffect);
			}
		}
	}

	SelfActor->Destroy();
}