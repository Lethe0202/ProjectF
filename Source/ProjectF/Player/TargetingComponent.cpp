// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingComponent.h"

#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectF/Character/PFCharacterBase.h"

UTargetingComponent::UTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TargetingWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetingWidgetComponent"));
	TargetingWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	TargetingWidgetComponent->SetDrawSize(FVector2D(500.f, 500.f));
	TargetingWidgetComponent->SetWidgetClass(TargetingMarkWidgetClass);	
}

void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner())
	{
		return;
	}
	
	OwnerCameraComponent = GetOwner()->FindComponentByClass<UCameraComponent>();
	OwnerCameraSpringArmComponent = GetOwner()->FindComponentByClass<USpringArmComponent>();

	OwnerCharacter = Cast<APFCharacterBase>(GetOwner());
	
	TargetingWidgetComponent->SetVisibility(false);
}

bool UTargetingComponent::CheckTargeting(TArray<FHitResult>& OutHitResults)
{
	FCollisionShape Shape;
	Shape = FCollisionShape::MakeSphere(CheckRangeSphereRadius);
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	FCollisionObjectQueryParams CollisionObjectQueryParams;
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	
	TArray<FHitResult> Results;
	
	bool bHit =  GetWorld()->SweepMultiByObjectType(Results, GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation(), FQuat::Identity, CollisionObjectQueryParams, Shape, Params);
	OutHitResults = Results;
	
	UKismetSystemLibrary::DrawDebugSphere(GetOwner()->GetWorld(), GetOwner()->GetActorLocation(), CheckRangeSphereRadius, 12, FColor::Red, 3, 1.f);
	return bHit;
}

void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateLockOnCamera();
}

void UTargetingComponent::SetupPlayerInputTargetingComponent(ULocalPlayer* LocalPlayer, UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void UTargetingComponent::ToggleTargeting()
{
	if (LockOnTarget)
	{
		ResetCamera();
		return;
	}
	
	TArray<FHitResult> HitResults;
	bool bHit = CheckTargeting(HitResults);
	
	if (bHit)
	{
		FVector OwnerLocation = GetOwner()->GetActorLocation();
		FRotator ControllerRotation = OwnerCharacter->Controller->GetControlRotation();
		
		AActor* CacheTarget = nullptr;
		float CacheDotProduct = -2.f;

		const IGenericTeamAgentInterface* OwnerTeamAgent = Cast<const IGenericTeamAgentInterface>(GetOwner());
		
		for (auto& HitResult : HitResults)
		{
			if (OwnerTeamAgent->GetTeamAttitudeTowards(*HitResult.GetActor()) != ETeamAttitude::Hostile) continue;
			
			if (CacheTarget == nullptr)
			{
				FVector NewDirectionVectorToTarget = (HitResult.GetActor()->GetActorLocation() - OwnerLocation).GetSafeNormal();
				float NewDotProduct = FVector::DotProduct(ControllerRotation.Vector(), NewDirectionVectorToTarget);
				
				CacheDotProduct = NewDotProduct;
				CacheTarget = HitResult.GetActor();
			}
			else
			{
				FVector NewDirectionVectorToTarget = (HitResult.GetActor()->GetActorLocation() - OwnerLocation).GetSafeNormal();
				float NewDotProduct = FVector::DotProduct(ControllerRotation.Vector(), NewDirectionVectorToTarget);
				
				if (CacheDotProduct < NewDotProduct)
				{
					CacheTarget = HitResult.GetActor();
					CacheDotProduct = NewDotProduct;
				}
			}
		}
		
		LockOnTarget = CacheTarget;
		if (LockOnTarget)
		{
			if (Cast<ACharacter>(LockOnTarget))
			{
				TargetingWidgetComponent->AttachToComponent(Cast<ACharacter>(LockOnTarget)->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("pelvis"));			
			}
			else
			{
				TargetingWidgetComponent->AttachToComponent(LockOnTarget->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
			}
		
			TargetingWidgetComponent->SetVisibility(true);
		}
	}

	if (OwnerCharacter)
	{
		OwnerCharacter->SetControlledMovement(false);
	}
}

void UTargetingComponent::LeftTargeting()
{
	if (!LockOnTarget)
	{
		return;
	}
	
	TArray<FHitResult> HitResults;
	bool bHit = CheckTargeting(HitResults);
	
	if (bHit)
	{
		FVector OwnerLocation = GetOwner()->GetActorLocation();
		float ControllerYaw = OwnerCharacter->Controller->GetControlRotation().Yaw; 

		AActor* CacheTarget = nullptr;
		float CacheMinAbsAngleDifference = 180.0f; 

		const IGenericTeamAgentInterface* OwnerTeamAgent = Cast<const IGenericTeamAgentInterface>(GetOwner());
		
		for (auto& HitResult : HitResults)
		{
			if (OwnerTeamAgent->GetTeamAttitudeTowards(*HitResult.GetActor()) != ETeamAttitude::Hostile) continue;
			
			AActor* NewHitActor = HitResult.GetActor();
			
			if (NewHitActor == nullptr) continue;
			if (NewHitActor == LockOnTarget) continue;
			
			FVector DirectionToCurrentActor = (NewHitActor->GetActorLocation() - OwnerLocation).GetSafeNormal();
			float TargetYaw = DirectionToCurrentActor.Rotation().Yaw;
			
			float CurrentAngleDelta = FMath::FindDeltaAngleDegrees(ControllerYaw, TargetYaw);
			
			if (CurrentAngleDelta < 0 && FMath::Abs(CurrentAngleDelta) < CacheMinAbsAngleDifference)
			{
				CacheTarget = NewHitActor;
				CacheMinAbsAngleDifference = FMath::Abs(CurrentAngleDelta);
			}
		}

		if (CacheTarget)
		{
			LockOnTarget = CacheTarget;
			if (Cast<ACharacter>(LockOnTarget))
			{
				TargetingWidgetComponent->AttachToComponent(Cast<ACharacter>(LockOnTarget)->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("pelvis"));			
			}
			else
			{
				TargetingWidgetComponent->AttachToComponent(LockOnTarget->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
			}
		}
	}
}

void UTargetingComponent::RightTargeting()
{
	if (!LockOnTarget)
	{
		return;
	}
	
	TArray<FHitResult> HitResults;
	bool bHit = CheckTargeting(HitResults);
	
	if (bHit)
	{
		FVector OwnerLocation = GetOwner()->GetActorLocation();
		float ControllerYaw = OwnerCharacter->Controller->GetControlRotation().Yaw; 

		AActor* CacheTarget = nullptr;
		float CacheMinAbsAngleDifference = 180.0f; 

		const IGenericTeamAgentInterface* OwnerTeamAgent = Cast<const IGenericTeamAgentInterface>(GetOwner());
		
		for (auto& HitResult : HitResults)
		{
			if (OwnerTeamAgent->GetTeamAttitudeTowards(*HitResult.GetActor()) != ETeamAttitude::Hostile) continue;
			
			AActor* NewHitActor = HitResult.GetActor();
			
			if (NewHitActor == nullptr) continue;
			if (NewHitActor == LockOnTarget) continue;
			
			FVector DirectionToCurrentActor = (NewHitActor->GetActorLocation() - OwnerLocation).GetSafeNormal();
			float TargetYaw = DirectionToCurrentActor.Rotation().Yaw;
			
			float CurrentAngleDelta = FMath::FindDeltaAngleDegrees(ControllerYaw, TargetYaw);
			
			if (CurrentAngleDelta > 0 && FMath::Abs(CurrentAngleDelta) < CacheMinAbsAngleDifference)
			{
				CacheTarget = NewHitActor;
				CacheMinAbsAngleDifference = FMath::Abs(CurrentAngleDelta);
			}
		}

		if (CacheTarget)
		{
			LockOnTarget = CacheTarget;
			if (Cast<ACharacter>(LockOnTarget))
			{
				TargetingWidgetComponent->AttachToComponent(Cast<ACharacter>(LockOnTarget)->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("pelvis"));			
			}
			else
			{
				TargetingWidgetComponent->AttachToComponent(LockOnTarget->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
			}
		}
	}
}

void UTargetingComponent::SetTarget(AActor* Target)
{
	LockOnTarget = Target;
	if (Cast<ACharacter>(LockOnTarget))
	{
		TargetingWidgetComponent->AttachToComponent(Cast<ACharacter>(LockOnTarget)->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("pelvis"));			
	}
	else
	{
		TargetingWidgetComponent->AttachToComponent(LockOnTarget->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

bool UTargetingComponent::IsActivateTargeting()
{
	return LockOnTarget != nullptr; 
}

void UTargetingComponent::UpdateNearestTargetActor()
{
}

void UTargetingComponent::UpdateLockOnCamera()
{
	if (!LockOnTarget) return;

	FVector TargetLocation = LockOnTarget->GetActorLocation();
	FVector OwnerLocation = GetOwner()->GetActorLocation();
	
	// 목표 방향
	FRotator LookAtRotation = (TargetLocation - OwnerLocation).Rotation();
	
	// 현재 카메라 회전
	FRotator CurrentRotation = Cast<APawn>(GetOwner())->GetController()->GetControlRotation();
	
	// 부드럽게 회전 보간
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, LookAtRotation, GetWorld()->GetDeltaSeconds(), CameraInterpSpeed);
	
	// 카메라 회전 적용
	Cast<APawn>(GetOwner())->GetController()->SetControlRotation(NewRotation);
	
	float DistanceToTarget = FVector::Distance(LockOnTarget->GetActorLocation(), OwnerLocation);
	float Offset = FMath::Clamp(DistanceToTarget / 10.f, 100.f ,DistanceToTarget / 10.f);
	
	OwnerCameraSpringArmComponent->SocketOffset = FVector(-Offset, 0, Offset * 2.5f);
	
	float PitchRotation = FMath::Max(-1 * DistanceToTarget / 100.f, -180);
	//대상과 플레이어가 원활하게 보이도록 카메라 로컬 회전
	float FinalPitchRotation = FMath::Min(-20.f, PitchRotation);
	OwnerCameraComponent->SetRelativeRotation(FRotator(FinalPitchRotation, 0.f, 0.f));
}

void UTargetingComponent::ResetCamera()
{
	LockOnTarget = nullptr;
	OwnerCameraSpringArmComponent->SocketOffset = FVector(0.f, 50.f, 100.f);
	OwnerCameraComponent->SetRelativeRotation(FRotator::ZeroRotator);
	TargetingWidgetComponent->SetVisibility(false);
	
	if (OwnerCharacter)
	{
		OwnerCharacter->SetControlledMovement(true);
	}
}

