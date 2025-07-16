// Fill out your copyright notice in the Description page of Project Settings.


#include "PFCharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "ProjectF/Character/PFCharacterBase.h"
#include "ProjectF/Character/State/ICombatState.h"
#include "ProjectF/Manager/AnimManager.h"
#include "ProjectF/Manager/PFGameInstance.h"


UPFCharacterMovementComponent::UPFCharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPFCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<APFCharacterBase>(CharacterOwner);
}

void UPFCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPFCharacterMovementComponent::PhysWalking(float deltaTime, int32 Iterations)
{
	Super::PhysWalking(deltaTime, Iterations);

	ECombatState CurrentState = OwnerCharacter->GetCurrentState();
	
	if (CurrentState== ECombatState::Launch)
	{
		OwnerCharacter->ChangeState(ECombatState::TakeDown);
	}
	else if (CurrentState == ECombatState::TakeDown)
	{
		TakeDownDurationTime += deltaTime;
		
		if (TakeDownDurationTime >= 1.f)
		{
			TakeDownDurationTime = 0.f;
			
			PlayMontageTakeDownRecover();
		}
	}
}

void UPFCharacterMovementComponent::PlayMontageTakeDownRecover()
{
	if (!OwnerCharacter->GetMesh())
	{
		return;
	}

	if (!OwnerCharacter->GetGameInstance())
	{
		return;
	}

	if (UPFGameInstance* PFGameInstance = Cast<UPFGameInstance>(OwnerCharacter->GetGameInstance()))
	{
		if (!PFGameInstance->GetAnimManager())
		{
			return;
		}

		if (UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance())
		{
			UAnimMontage* TakeDownRecoverMontage = PFGameInstance->GetAnimManager()->GetTakeDownRecover(OwnerCharacter->GetID());
			if (TakeDownRecoverMontage)
			{
				if (AnimInstance->GetCurrentActiveMontage() != TakeDownRecoverMontage)
				{
					AnimInstance->Montage_Play(TakeDownRecoverMontage);	
				}
			}
		}
	}
}

void UPFCharacterMovementComponent::PhysFalling(float deltaTime, int32 Iterations)
{
	Super::PhysFalling(deltaTime, Iterations);
	
	TakeDownDurationTime = 0.f;
	InAirDurationTime = deltaTime;
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Falling"));
}



