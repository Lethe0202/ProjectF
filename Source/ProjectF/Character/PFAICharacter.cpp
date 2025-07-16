// Fill out your copyright notice in the Description page of Project Settings.


#include "PFAICharacter.h"

#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Component/HealthComponent.h"
#include "Component/StaminaComponent.h"
#include "Components/WidgetComponent.h"
#include "ProjectF/Controller/PFAIController.h"
#include "ProjectF/Interaction/DialogueComponent.h"
#include "ProjectF/Manager/AnimManager.h"
#include "ProjectF/Manager/PFGameInstance.h"
#include "ProjectF/UI/HUD/ExecutionProgressWidget.h"

APFAICharacter::APFAICharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComponent"));
	HealthWidgetComponent->SetupAttachment(GetMesh());
	
	ExecutionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ExecutionWidgetComponent"));
	ExecutionWidgetComponent->SetupAttachment(GetMesh(), TEXT("pelvis"));
}

void APFAICharacter::BeginPlay()
{
	Super::BeginPlay();

	ExecutionWidgetComponent->SetVisibility(false);
	
	UUserWidget* ExecutionUserWidget = ExecutionWidgetComponent->GetWidget();
	if (!ExecutionUserWidget) return;
	
	UExecutionProgressWidget* ExecutionWidget= Cast<UExecutionProgressWidget>(ExecutionUserWidget);
	if (!ExecutionWidget) return;
	
	ExecutionCooldownDelegate.AddDynamic(ExecutionWidget, &UExecutionProgressWidget::OnUpdateHoldProgress);
}

float APFAICharacter::ApplyStamina(float InStamina)
{
	if (bExecuted) return 0.f;
	if (!StaminaComponent) return 0.f;
	
	StaminaComponent->AddStamina(InStamina);

	if (StaminaComponent->GetCurrentStamina() > 0) return InStamina;
	
	ExecutionWidgetComponent->SetVisibility(true);
	ExecutionCooldownStartTime = GetWorld()->GetTimeSeconds();
	GetWorld()->GetTimerManager().SetTimer(ExecutionCooldownTimer, this, &ThisClass::HandleExecutionCooldownTimer, GetWorld()->GetDeltaSeconds(), true);
	
	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance) return InStamina;
	
	UPFGameInstance* PFGameInstance = Cast<UPFGameInstance>(GameInstance);
	if (!PFGameInstance) return InStamina;

	UAnimManager* AnimManager = PFGameInstance->GetAnimManager();
	if (!AnimManager) return InStamina;
			
	if (UAnimMontage* CurrentMontage = GetMesh()->GetAnimInstance()->GetCurrentActiveMontage())
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.0f, CurrentMontage);		
	}
			
	// 블렌딩 오류 방지를 위한 1프레임 후 애니메이션 재생
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		UGameInstance* GameInstance = GetGameInstance();
		if (!GameInstance) return;

		UPFGameInstance* PFGameInstance = Cast<UPFGameInstance>(GameInstance);
		if (!PFGameInstance) return;
				
		UAnimManager* AnimManager = PFGameInstance->GetAnimManager();
		if (!AnimManager) return;
				
		if (UAnimMontage* StunAnimMontage = AnimManager->GetStun(GetID()))
		{
			GetMesh()->GetAnimInstance()->Montage_Play(StunAnimMontage);
		}
	}, GetWorld()->GetDeltaSeconds(), false);

	bExecuted = true;
	
	return InStamina;
}

void APFAICharacter::HandleExecutionCooldownTimer()
{
	float ElapsedTime = GetWorld()->GetTimeSeconds() - ExecutionCooldownStartTime;
	
	if (ElapsedTime >= 3.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(ExecutionCooldownTimer);
		OnExecutedEnd();
		ExecutionCooldownDelegate.Broadcast(0.f, 0.f);	//clear
	}
	else
	{
		ExecutionCooldownDelegate.Broadcast(3.f, ElapsedTime);
	}
}

void APFAICharacter::ModifyDamage(float DamageAmount, AActor* InInstigator)
{
	ApplyStamina(-DamageAmount);
	HealthComponent->ApplyDamage(DamageAmount, InInstigator);
}

AActor* APFAICharacter::GetTarget() const
{
	APFAIController* PFController = Cast<APFAIController>(Controller);
	if (!PFController) return nullptr;
	
	UBlackboardComponent* BlackboardComponent = PFController->GetBlackboardComponent();
	if (!BlackboardComponent) return nullptr;
	
	if (UObject* TargetObject = BlackboardComponent->GetValueAsObject("TargetCharacter"))
	{
		if (AActor* TargetActor = Cast<AActor>(TargetObject))
		{
			return TargetActor;
		}
	}
	
	return nullptr;
}

void APFAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APFAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APFAICharacter::Interact_Implementation(AActor* InteractingActor)
{
	if (UDialogueComponent* DialogueComponent = FindComponentByClass<UDialogueComponent>())
	{
		DialogueComponent->StartDialogue(InteractingActor);
	}
}

void APFAICharacter::StopLogic(FString Reason)
{
	Super::StopLogic(Reason);
	
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController && AIController->GetBrainComponent())
	{
		AIController->GetBrainComponent()->PauseLogic(Reason);
	}
}

void APFAICharacter::Executed()
{
	Super::Executed();
	
	GetWorld()->GetTimerManager().ClearTimer(ExecutionCooldownTimer);
	ExecutionCooldownDelegate.Broadcast(0.f, 0.f);	//clear
}

void APFAICharacter::OnExecutedEnd()
{
	Super::OnExecutionEnd();

	bExecuted = false;
	ApplyStamina(99999.f);
	GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("LoopEnd"));
}

