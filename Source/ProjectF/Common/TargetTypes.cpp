#include "TargetTypes.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
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

	FCollisionResponseParams CollisionResponseParams;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQuery;
	ObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	EDrawDebugTrace::Type DebugTrace = EDrawDebugTrace::None; 
	
	if (!bDebug)
	{
		DebugTrace = EDrawDebugTrace::None;
	}
	else
	{
		DebugTrace = DrawDebugTrace;
	}
	
	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		TargetingActor->GetWorld(), TraceStartLocation, TraceEndLocation, Radius, ObjectTypeQuery,
		false, IgnoreActors, DebugTrace, Results, true, FColor::Red, FColor::Green, Duration);
	
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
