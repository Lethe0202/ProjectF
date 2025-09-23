// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingComponent.h"

#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	OwnerController = OwnerCharacter->GetController();
	
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
	
	return bHit;
}

void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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

	if (!OwnerCharacter->Controller) return;
	
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
			
			ACharacter* HitCharacter = Cast<ACharacter>(HitResult.GetActor());
			if (!HitCharacter) continue;
			if (HitCharacter->IsPawnControlled() == false) continue;
			
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
			
			FRotator Rotator = FRotator::ZeroRotator;
			Rotator.Pitch = OwnerController->GetControlRotation().Pitch;
			OwnerCameraComponent->SetRelativeRotation(Rotator);
			
			if (OwnerCharacter)
			{
				OwnerCharacter->SetControlledMovement(false);
			}
		}
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
	if (!LockOnTarget)
	{
		return;
	}

	if (!OwnerCharacter) return;
	if (!OwnerController) return;
	
	ACharacter* HitCharacter = Cast<ACharacter>(LockOnTarget);
	if (!HitCharacter)
	{
		ResetCamera();
		return;
	}
	
	if (HitCharacter->IsPawnControlled() == false)
	{
		ResetCamera();
        return;
	}
	
	FVector TargetLocation = LockOnTarget->GetActorLocation();
	FVector OwnerLocation = OwnerCharacter->GetActorLocation();
	
	float DistanceToTarget = FVector::Distance(LockOnTarget->GetActorLocation(), OwnerLocation);
	if (DistanceToTarget > CheckRangeSphereRadius)
	{
		ResetCamera();
		return;
	}
	
	// 목표 방향
	FRotator LookAtRotation = (TargetLocation - OwnerLocation).Rotation();
	
	// 현재 카메라 회전
	FRotator CurrentRotation = OwnerController->GetControlRotation();
	
	// 부드럽게 회전 보간
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, LookAtRotation, GetWorld()->GetDeltaSeconds(), CameraInterpSpeed);

	// 카메라 회전 clamp
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch , -20.f, 20.f);
	
	// 카메라 회전 적용
	OwnerController->SetControlRotation(NewRotation);
	
	float Offset = FMath::Clamp(DistanceToTarget / 10.f, 100.f ,DistanceToTarget / 10.f);
	
	OwnerCameraSpringArmComponent->SocketOffset = FVector(-Offset, 0, Offset * 2.5f);

	//대상과 플레이어가 원활하게 보이도록 카메라 로컬 회전
	float PitchRotation = FMath::Max(-1 * DistanceToTarget / 100.f, -180);
	float FinalPitchRotation = FMath::Min(-20.f, PitchRotation);
	OwnerCameraComponent->SetRelativeRotation(FRotator(FinalPitchRotation, 0.f, 0.f));
}

void UTargetingComponent::ResetCamera()
{
	OwnerCameraSpringArmComponent->SocketOffset = FVector(0.f, 50.f, 100.f);
	
	FRotator Rotator;
	Rotator.Pitch = OwnerCameraComponent->GetRelativeRotation().Pitch;
	Rotator.Roll = OwnerController->GetControlRotation().Roll;
	Rotator.Yaw = OwnerController->GetControlRotation().Yaw;
	OwnerController->SetControlRotation(Rotator);
	
	OwnerCameraComponent->SetRelativeRotation(FRotator::ZeroRotator);

	if (OwnerCharacter)
    {
    	OwnerCharacter->SetControlledMovement(true);
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 600.f;
    }
	
	LockOnTarget = nullptr;
	TargetingWidgetComponent->SetVisibility(false);
}

