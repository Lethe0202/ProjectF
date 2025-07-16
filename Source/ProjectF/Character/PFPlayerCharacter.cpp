// Fill out your copyright notice in the Description page of Project Settings.


#include "PFPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Component/CombatStateComponent.h"
#include "Component/HealthComponent.h"
#include "Component/StaminaComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectF/Ability/AbilityBase.h"

#include "ProjectF/Player/TargetingComponent.h"
#include "ProjectF/Character/Component/AbilityComponent.h"
#include "ProjectF/Interaction/Interactable.h"
#include "ProjectF/Weapon/WeaponBase.h"

class UEnhancedInputLocalPlayerSubsystem;

APFPlayerCharacter::APFPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	TargetingComponent = CreateDefaultSubobject<UTargetingComponent>(TEXT("TargetingComponent"));
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	InteractionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphereComponent"));
	InteractionSphereComponent->SetupAttachment(RootComponent);
	InteractionSphereComponent->InitSphereRadius(300.f);
	
	InteractionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APFPlayerCharacter::HandleInteractionSphereBeginOverlap);
	InteractionSphereComponent->OnComponentEndOverlap.AddDynamic(this, &APFPlayerCharacter::HandleInteractionSphereEndOverlap);

	ExecutionInteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ExecutionInteractionWidgetComponent"));
	ExecutionInteractionWidgetComponent->SetVisibility(false);
}

void APFPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AbilityInputDataMap.Add(0, FAbilityInputData());
	AbilityInputDataMap.Add(1, FAbilityInputData());
	AbilityInputDataMap.Add(2, FAbilityInputData());
	AbilityInputDataMap.Add(3, FAbilityInputData());

	if(GuardCounterAbilityClass)
	{
		GuardCounterAbility = NewObject<UAbilityBase>(this, GuardCounterAbilityClass);
		AbilityComponent->RegisterAbility(GuardCounterAbility);
	}

	GetWorld()->GetTimerManager().SetTimer(ExecutionTimerHandle, this, &APFPlayerCharacter::HandleExecutionCollisionTimer, 0.1f, true);
}

void APFPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APFPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (TargetingComponent)
		{
			TargetingComponent->SetupPlayerInputTargetingComponent(PlayerController->GetLocalPlayer(), PlayerInputComponent);
		}
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			Subsystem->AddMappingContext(AbilityInputMappingContext, 0);
		}
	
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APFPlayerCharacter::Input_Move);
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APFPlayerCharacter::Input_Look);
			EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &APFPlayerCharacter::Input_Roll);
			
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APFPlayerCharacter::Input_Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APFPlayerCharacter::Input_JumpStop);
			
			EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APFPlayerCharacter::Input_Attack);
			EnhancedInputComponent->BindAction(StrongAttackAction, ETriggerEvent::Started, this, &APFPlayerCharacter::Input_StrongAttack);
			EnhancedInputComponent->BindAction(StrongAttackAction, ETriggerEvent::Triggered, this, &APFPlayerCharacter::Input_AttackHold);
			EnhancedInputComponent->BindAction(StrongAttackAction, ETriggerEvent::Completed, this, &APFPlayerCharacter::Input_AttackHoldStop);
			
			EnhancedInputComponent->BindAction(Ability1Action, ETriggerEvent::Started, this, &APFPlayerCharacter::Input_AbilityPressed, 0);
			EnhancedInputComponent->BindAction(Ability2Action, ETriggerEvent::Started, this, &APFPlayerCharacter::Input_AbilityPressed, 1);
			EnhancedInputComponent->BindAction(Ability3Action, ETriggerEvent::Started, this, &APFPlayerCharacter::Input_AbilityPressed, 2);
			EnhancedInputComponent->BindAction(Ability4Action, ETriggerEvent::Started, this, &APFPlayerCharacter::Input_AbilityPressed, 3);
			
			EnhancedInputComponent->BindAction(Ability1Action, ETriggerEvent::Triggered, this, &APFPlayerCharacter::Input_AbilityHold, 0);
			EnhancedInputComponent->BindAction(Ability2Action, ETriggerEvent::Triggered, this, &APFPlayerCharacter::Input_AbilityHold, 1);
			EnhancedInputComponent->BindAction(Ability3Action, ETriggerEvent::Triggered, this, &APFPlayerCharacter::Input_AbilityHold, 2);
			EnhancedInputComponent->BindAction(Ability4Action, ETriggerEvent::Triggered, this, &APFPlayerCharacter::Input_AbilityHold, 3);
			
			EnhancedInputComponent->BindAction(Ability1Action, ETriggerEvent::Completed, this, &APFPlayerCharacter::Input_AbilityReleased, 0);
			EnhancedInputComponent->BindAction(Ability2Action, ETriggerEvent::Completed, this, &APFPlayerCharacter::Input_AbilityReleased, 1);
			EnhancedInputComponent->BindAction(Ability3Action, ETriggerEvent::Completed, this, &APFPlayerCharacter::Input_AbilityReleased, 2);
			EnhancedInputComponent->BindAction(Ability4Action, ETriggerEvent::Completed, this, &APFPlayerCharacter::Input_AbilityReleased, 3);
			
			EnhancedInputComponent->BindAction(GuardAction, ETriggerEvent::Started, this, &APFPlayerCharacter::Input_Guard);
			EnhancedInputComponent->BindAction(GuardAction, ETriggerEvent::Completed, this, &APFPlayerCharacter::Input_GuardReleased);
			EnhancedInputComponent->BindAction(ExecutionAction, ETriggerEvent::Started, this, &APFPlayerCharacter::Input_Execution);
			
			EnhancedInputComponent->BindAction(HealAction, ETriggerEvent::Started, this, &APFPlayerCharacter::Input_Heal);
		}
	}
}

void APFPlayerCharacter::Input_Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator ControllerRotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, ControllerRotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
	
	CurMovementDirection = ((ForwardDirection * MovementVector.Y) + (RightDirection * MovementVector.X)).GetSafeNormal();
}

void APFPlayerCharacter::Input_Look(const FInputActionValue& Value)
{
	FVector2D LookDirection = Value.Get<FVector2D>();
	
	AddControllerYawInput(LookDirection.X);
	AddControllerPitchInput(LookDirection.Y);
}

void APFPlayerCharacter::Input_Roll(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->MovementMode != MOVE_Walking) return;
	
	FVector ForwardVector = GetControlRotation().Vector();

	float DotProduct = FVector::DotProduct(ForwardVector, CurMovementDirection);
	float Angle = FMath::Acos(DotProduct) * (180.f / PI);
	
	// 방향 판별용 (좌우)
	float CrossZ = FVector::CrossProduct(ForwardVector, CurMovementDirection).Z;

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
	
	RollCharacter(CharacterDirection);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Roll: %f"), Angle));
}

void APFPlayerCharacter::Input_Jump(const FInputActionValue& Value)
{
	if (CombatStateComponent->GetCurrentState() == ECombatState::Idle)
	{
		Jump();	
	}
}

void APFPlayerCharacter::Input_JumpStop(const FInputActionValue& Value)
{
	StopJumping();
}

void APFPlayerCharacter::Input_Attack(const FInputActionValue& Value)
{
	TryAttack();
}

void APFPlayerCharacter::Input_StrongAttack(const FInputActionValue& Value)
{
	if (bCanGuardCounterAttack)
	{
		if (ChangeState(ECombatState::Attacking))
		{
			GetMesh()->GetAnimInstance()->Montage_Play(GuardCounterMontage);
			
			GetWorld()->GetTimerManager().ClearTimer(GuardCounterAttackTimerHandle);
			bCanGuardCounterAttack = false;
		}
	}
	else
	{
		TryStrongAttack();
	}
}

void APFPlayerCharacter::Input_AttackHold(const FInputActionValue& Value)
{
	AttackHoldTime += GetWorld()->GetDeltaSeconds();
	AttackHold(AttackHoldTime);
}

void APFPlayerCharacter::Input_AttackHoldStop(const FInputActionValue& Value)
{
	bool bSuccessAttackHold = TryAttackHold(AttackHoldTime);
	if (bSuccessAttackHold)
	{
		StaminaComponent->AddStamina(-15.f);
	}
	
	AttackHoldTime = 0.f;
}

void APFPlayerCharacter::Input_AbilityPressed(int Index)
{
	if (AbilityInputDataMap.Contains(Index))
	{
		AbilityInputDataMap[Index] = FAbilityInputData();
		
		FAbilityInputData& AbilityInputData = AbilityInputDataMap[Index];
		AbilityInputData.StartHoldTime = GetWorld()->GetTimeSeconds();
	}
	
	ActivateAbility(Index);
}

void APFPlayerCharacter::Input_AbilityHold(int Index)
{
	if (AbilityInputDataMap.Contains(Index))
	{
		FAbilityInputData& AbilityInputData = AbilityInputDataMap[Index];
		AbilityInputData.CurrentHoldingTime = GetWorld()->GetTimeSeconds() - AbilityInputData.StartHoldTime;
		OnHoldingAbility(Index, AbilityInputData.CurrentHoldingTime);
	}
}

void APFPlayerCharacter::Input_AbilityReleased(int Index)
{
	if (AbilityInputDataMap.Contains(Index))
	{
		FAbilityInputData& AbilityInputData = AbilityInputDataMap[Index];
		OnReleasedAbility(Index, AbilityInputData.CurrentHoldingTime);
	}
}

void APFPlayerCharacter::Input_Guard(const FInputActionValue& Value)
{
	if (!Weapon) return;
	
	if (CombatStateComponent->CanChangeState(ECombatState::Guard))
	{
		if (Weapon->TryGuard())
		{
			CombatStateComponent->ChangeState(ECombatState::Guard);
		}
	}
}

void APFPlayerCharacter::Input_GuardReleased(const FInputActionValue& Value)
{
	if (!Weapon) return;
	
	if (CombatStateComponent->GetCurrentState() == ECombatState::Guard)
	{
		if (Weapon->ReleaseGuard())
		{
			CombatStateComponent->ChangeState(ECombatState::Idle);
		}
	}
}

void APFPlayerCharacter::Input_Execution(const FInputActionValue& Value)
{
	if (!ExecutionTargetCharacter) return;
	if (bPlayExecution) return;
	if (GetCharacterMovement()->MovementMode != MOVE_Walking) return;
	
	if (ExecutionMontage)
	{
		if (TargetingComponent->GetTarget())
		{
			TargetingComponent->SetTarget(ExecutionTargetCharacter);	
		}
		
		bPlayExecution = true;
		
		FRotator LookAtRotation = (ExecutionTargetCharacter->GetActorLocation() - GetActorLocation()).Rotation();
		SetActorRotation(LookAtRotation);
		
		ExecutionTargetCharacter->Executed();
		GetMesh()->GetAnimInstance()->Montage_Play(ExecutionMontage);
		
		ExecutionInteractionWidgetComponent->SetVisibility(false);
	}
}

void APFPlayerCharacter::Input_Heal()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(HealingCooldownTimerHandle)) return;

	if (CombatStateComponent->GetCurrentState() != ECombatState::Idle) return;
	
	if (HealthComponent)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			if (HealingMontage)
			{
				AnimInstance->Montage_Play(HealingMontage, 1.0f); // 1.0f는 재생 속도
			}
		}
	}
}

void APFPlayerCharacter::ApplyHeal()
{
	HealthComponent->ApplyHeal(HealingItemAmount);
	PotionCooldownStartTime = GetWorld()->GetTimeSeconds();
	GetWorld()->GetTimerManager().SetTimer(HealingCooldownTimerHandle, this, &ThisClass::HandleUpdatePotionCooldown, GetWorld()->GetDeltaSeconds(), true);
}

float APFPlayerCharacter::ApplyStamina(float InStamina)
{
	return 0.f;
}

void APFPlayerCharacter::ModifyDamage(float DamageAmount, AActor* InInstigator)
{
	if (CombatStateComponent->GetCurrentState() == ECombatState::Guard)
	{
		if (CombatStateComponent->IsGuardCounter())
		{
			SuccessGuardCounter(InInstigator);
		}
		else
		{
			SuccessGuard(-DamageAmount);
		}
	}
	else
	{
		HealthComponent->ApplyDamage(DamageAmount, InInstigator);
	}
}

void APFPlayerCharacter::HandleUpdatePotionCooldown()
{
	float ElapsedTime = GetWorld()->GetTimeSeconds() - PotionCooldownStartTime;

	if (ElapsedTime >= HealingItemCooldown)
	{
		GetWorld()->GetTimerManager().ClearTimer(HealingCooldownTimerHandle);
	}
	
	OnUpdatePotionCooldown.Broadcast(HealingItemCooldown, ElapsedTime);
}

void APFPlayerCharacter::SuccessGuardCounter(AActor* InInstigator)
{
	Super::SuccessGuardCounter(InInstigator);
	
	bParrySuccess = true;
	
	// 패링에 성공하면 다른 Effect를 무시하기 위해 1프레임 동안 패링 flag true 
	FTimerHandle ResetParryInvincibleTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		ResetParryInvincibleTimerHandle,
		this,
		&APFPlayerCharacter::ResetParryInvincible,
		GetWorld()->GetDeltaSeconds(),
		false
	);
	
	GetMesh()->GetAnimInstance()->Montage_Play(GetWeapon()->GetGuardCounterMontage());
	ChangeState(ECombatState::Idle);

	// 가드 카운터 어택 flag true
	bCanGuardCounterAttack = true;
	GetWorld()->GetTimerManager().SetTimer(GuardCounterAttackTimerHandle,
		[this]()
		{
			bCanGuardCounterAttack = false;
		},
		GuardCounterAttackDuration, false);

	CurrentCounterTarget = InInstigator;
}

void APFPlayerCharacter::SuccessGuard(float InStaminaValue)
{
	Super::SuccessGuard(InStaminaValue);
	
	StaminaComponent->AddStamina(InStaminaValue);
	
	if (UAnimMontage* GuardSuccessMontage = Weapon->GetGuardSuccessMontage())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(GuardSuccessMontage);
	}
}

void APFPlayerCharacter::SuccessAbilityCounter()
{
	Super::SuccessAbilityCounter();

	if (TargetingComponent->IsActivateTargeting())
	{
		TargetingComponent->ToggleTargeting();
		GetCharacterMovement()->MaxWalkSpeed = 600.f;	
	}
}

void APFPlayerCharacter::RotateMovement()
{
	SetActorRotation(CurMovementDirection.Rotation());
}

bool APFPlayerCharacter::TryInteraction()
{
	if (InteractableActors.IsValidIndex(0))
	{
		if (InteractableActors[0]->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			IInteractable::Execute_Interact(InteractableActors[0], this);
			return true;
		}
	}
	
	return false;
}

void APFPlayerCharacter::HandleExecutionCollisionTimer()
{
	if (bPlayExecution)
	{
		return;
	}
	
	// 기존에 저장되어있는 타겟 업데이트
	float NearestDistance = FLT_MAX;
	if (ExecutionTargetCharacter)
	{
		float TargetDistance = UKismetMathLibrary::Vector_Distance(ExecutionTargetCharacter->GetActorLocation(), GetActorLocation());
		if (TargetDistance > 100.f)
		{
			ExecutionTargetCharacter = nullptr;
			if (ExecutionInteractionWidgetComponent)
			{
				ExecutionInteractionWidgetComponent->SetVisibility(false);
			}
		}
		else
		{
			NearestDistance = TargetDistance;
		}
	}
	
	// 새로운 Sweep으로 타겟 업데이트
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	
	TArray<FHitResult> Results;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQuery;
	ObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	
	EDrawDebugTrace::Type DebugTrace = EDrawDebugTrace::None;
	
	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
	GetWorld(), GetActorLocation(), GetActorLocation(), 100.f, ObjectTypeQuery,
	false, IgnoreActors, DebugTrace, Results, true, FColor::Red, FColor::Green, 1.f);
	
	if (!bHit) return;
	
	for (auto& SweepResult : Results)
	{
		AActor* TargetActor = SweepResult.GetActor();
		if (!TargetActor) continue;
		
		APFCharacterBase* OtherPFCharacter = Cast<APFCharacterBase>(TargetActor);
		if (!OtherPFCharacter) continue;
	
		UStaminaComponent* OtherStaminaComponent = OtherPFCharacter->GetStaminaComponent();
		if (!OtherStaminaComponent) continue;
		
		if (OtherStaminaComponent->GetCurrentStamina() <= 0.f)
		{
			float TargetDistance= UKismetMathLibrary::Vector_Distance(GetActorLocation(), OtherPFCharacter->GetActorLocation());
			if (TargetDistance < NearestDistance)
			{
				ExecutionTargetCharacter = OtherPFCharacter;
				NearestDistance = TargetDistance;
			}
		}
	}

	if (ExecutionInteractionWidgetComponent)
	{
		if (ExecutionTargetCharacter)
		{
			ExecutionInteractionWidgetComponent->AttachToComponent(ExecutionTargetCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("pelvis"));
			ExecutionInteractionWidgetComponent->SetVisibility(true);
		}
		else
		{
			ExecutionInteractionWidgetComponent->SetVisibility(false);
		}
	}
}

void APFPlayerCharacter::OnExecutionTrigger()
{
	Super::OnExecutionTrigger();
	
	FVector ClosestLocation;
	GetMesh()->GetClosestPointOnCollision(ExecutionTargetCharacter->GetActorLocation(), ClosestLocation);

	FTransform EffectTransform;
	EffectTransform.SetLocation(ClosestLocation);
	
	TArray<TObjectPtr<UEffectType>> Effects = ExecutionDataAsset->EffectContainer[0].Effect;
	
	for (auto& Effect : Effects)
	{
		Effect->ApplyEffect(ExecutionTargetCharacter, this, EffectTransform ,false);
	}
}

void APFPlayerCharacter::OnExecutionEnd()
{
	Super::OnExecutionEnd();
	
	if (ExecutionTargetCharacter)
	{
		ExecutionTargetCharacter->OnExecutedEnd();
		ExecutionTargetCharacter = nullptr;
		
		bPlayExecution = false;
	}
}

void APFPlayerCharacter::HandleInteractionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		InteractableActors.AddUnique(OtherActor);
		IInteractable::Execute_VisibleInteractablePrompt(OtherActor, true);
	}
}

void APFPlayerCharacter::HandleInteractionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		InteractableActors.AddUnique(OtherActor);
		IInteractable::Execute_VisibleInteractablePrompt(OtherActor, true);
	}
	
	IInteractable* Interactable = Cast<IInteractable>(OtherActor);
	if (Interactable)
	{
		if (InteractableActors.Contains(OtherActor))
		{
			InteractableActors.Remove(OtherActor);
		}
		
		Interactable->Execute_VisibleInteractablePrompt(OtherActor, false);
	}
}

bool APFPlayerCharacter::TryAttack()
{
	if (bCanAttack && Weapon->GetGroundAttackCnt() == 0)
	{
		RotateMovement();
	}
	
	bool bAttackSuccess = Super::TryAttack();
	if (bAttackSuccess)
	{
		StaminaComponent->AddStamina(-10.f);
		return true;
	}
	else
	{
		return false;
	}
}

bool APFPlayerCharacter::TryStrongAttack()
{
	if (bCanAttack && Weapon->GetGroundAttackCnt() == 0)
	{
		RotateMovement();
	}
	
	if (Weapon->GetAttachParentSocketName() != "hand_r_socket")
	{
		return false;
	}
	
	if (bCanAttack && CanChangeState(ECombatState::Attacking))
	{
		if (Weapon->TryStrongAttack())
		{
			ChangeState(ECombatState::Attacking);
			bCanAttack = false;
			StaminaComponent->AddStamina(-15.f);

			return true;
		}
	}
	
	return false;
}

void APFPlayerCharacter::RollCharacter(ECharacterDirection Direction)
{
	if (TargetingComponent->GetTarget())
	{
		Super::RollCharacter(Direction);	
	}
}

void APFPlayerCharacter::OnHoldingAbility(int Index, float HoldingTime)
{
	if (AbilityComponent)
	{
		if (!GetMovementComponent()->IsFalling())	// 공중에서는 스킬 홀드 X
		{
			AbilityComponent->OnHoldingAbility(Index, HoldingTime);
		}
	}
}

void APFPlayerCharacter::OnReleasedAbility(int Index, float HoldingTime)
{
	if (AbilityComponent)
	{
		AbilityComponent->OnReleasedAbility(Index, HoldingTime);
	}
}

AActor* APFPlayerCharacter::GetTarget() const
{
	if (TargetingComponent)
	{
		return TargetingComponent->GetTarget();
	}

	return nullptr;
}
