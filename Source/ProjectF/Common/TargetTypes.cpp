#include "TargetTypes.h"

#include "KismetTraceUtils.h"
#include "PFCollisionChannel.h"
#include "GameFramework/Character.h"
#include "ProjectF/Character/PFCharacterBase.h"

void UTargetType::GetTargets_Implementation(AActor* TargetingActor, TArray<AActor*>& OutActors, TArray<FHitResult>& OutHitResults) const
{
	return;
}

void UTargetType_Self::GetTargets_Implementation(AActor* TargetingActor, TArray<AActor*>& OutActors, TArray<FHitResult>& OutHitResults) const
{
	OutActors.Emplace(TargetingActor);
}

void UTargetType_Sphere::GetTargets_Implementation(AActor* TargetingActor, TArray<AActor*>& OutActors, TArray<FHitResult>& OutHitResults) const
{
	FVector TraceOffset = TargetingActor->GetActorForwardVector() * Offset;
	FVector TraceStartLocation = TargetingActor->GetActorLocation() + TraceOffset;
	FVector TraceEndLocation = TargetingActor->GetActorLocation() + TraceOffset;
	
	if (ACharacter* Character = Cast<ACharacter>(TargetingActor))
	{
		FVector SocketStartLocation = Character->GetMesh()->GetSocketLocation(SocketName);
		if (!SocketStartLocation.IsZero())
		{
			TraceStartLocation = SocketStartLocation + TraceOffset;
		}

		FVector SocketEndLocation = Character->GetMesh()->GetSocketLocation(EndSocketName);
		if (!SocketEndLocation.IsZero())
		{
			TraceEndLocation = SocketEndLocation + TraceOffset;
		}
	}
	
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(TargetingActor);
	
	TArray<FHitResult> Results;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQuery;
	ObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECC_CharacterMesh));
	
	TArray<FHitResult> emptyResult;

#if WITH_EDITOR
	if (bDebug)
	{
		DrawDebugSphereTraceMulti(TargetingActor->GetWorld(), TraceStartLocation, TraceEndLocation, Radius, DrawDebugTrace, false, emptyResult, FColor::Red, FColor::Green, Duration);
	}
#endif
	
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(MyTrace), false);
	QueryParams.AddIgnoredActor(TargetingActor);
	QueryParams.bReturnPhysicalMaterial = true;
	
	bool bHit = TargetingActor->GetWorld()->SweepMultiByObjectType(
		Results,
		TraceStartLocation,
		TraceEndLocation,
		FQuat::Identity,
		ObjectTypeQuery,
		FCollisionShape::MakeSphere(Radius),
		QueryParams
	);
	
	if (bHit)
	{
		for (int i = 0; i < Results.Num(); ++i)
		{
			ACharacter* HitCharacter = Cast<ACharacter>(Results[i].GetActor());
			if (HitCharacter != nullptr)
			{
				OutActors.AddUnique(HitCharacter);
			}
		}
		OutHitResults.Append(Results);
	}
}

void UTargetType_Targeting::GetTargets_Implementation(AActor* TargetingActor, TArray<AActor*>& OutActors, TArray<FHitResult>& OutHitResults) const
{
	APFCharacterBase* PFCharacterBase = Cast<APFCharacterBase>(TargetingActor);
	if (!PFCharacterBase)
	{
		return;
	}
	
	if (PFCharacterBase->GetTarget())
	{
		FHitResult HitResult;
		HitResult.ImpactPoint = PFCharacterBase->GetTarget()->GetActorLocation();
		OutHitResults.Emplace(HitResult);
		OutActors.Emplace(PFCharacterBase->GetTarget());
	}
}
