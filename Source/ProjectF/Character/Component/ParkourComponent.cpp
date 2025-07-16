// Fill out your copyright notice in the Description page of Project Settings.


#include "ParkourComponent.h"

#include "CombatStateComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectF/Character/PFCharacterBase.h"

UParkourComponent::UParkourComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner() != nullptr)
	{
		Owner = Cast<ACharacter>(GetOwner());	
	}

	if (!Owner) return;
	
	APFCharacterBase* OwnerCharacter = Cast<APFCharacterBase>(Owner);
	MovementComponent = OwnerCharacter->GetCharacterMovement();
	CombatStateComponent = OwnerCharacter->GetCombatStateComponent();
}

void UParkourComponent::StartParkour_Implementation(AActor* TargetActor, FHitResult& HitResult)
{
}

void UParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MovementComponent->MovementMode != MOVE_Falling) return;
	if (CombatStateComponent->GetCurrentState() != ECombatState::Idle) return;
	
	FHitResult HitResult;
	
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	FVector StartLocation2 = OwnerCharacter->GetMesh()->GetSocketLocation(FName("head")) + OwnerCharacter->GetActorForwardVector() * 40.f;
	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(Owner);
	CollisionQueryParams.bDebugQuery = false;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQuery;
	ObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
	
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Owner);
	
	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartLocation2, StartLocation2 + GetOwner()->GetActorUpVector() * -1.f * 75.f, 15.f, ObjectTypeQuery, true, IgnoreActors, EDrawDebugTrace::ForOneFrame, HitResult, true, FColor::Red, FColor::Green, 1.f);
	if (bHit)
	{
		StartParkour(HitResult.GetActor(), HitResult);
	}
}

