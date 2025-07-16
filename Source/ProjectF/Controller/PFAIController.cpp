// Fill out your copyright notice in the Description page of Project Settings.


#include "PFAIController.h"
#include "ProjectF/Character/PFAICharacter.h"
#include "ProjectF/Character/PFCharacterBase.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"


// Sets default values
APFAIController::APFAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));
	
	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &APFAIController::OnUpdatePerception);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	
	SightConfig->SightRadius = 600.f;
	SightConfig->LoseSightRadius = 1000.f;
	SightConfig->PeripheralVisionAngleDegrees = 60.f;
	SightConfig->SetMaxAge(1.f);
	
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	PerceptionComponent->ConfigureSense(*SightConfig);
	
	DamageConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));
	PerceptionComponent->ConfigureSense(*DamageConfig);
}

void APFAIController::BeginPlay()
{
	Super::BeginPlay();
}

void APFAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APFAIController::StartTree()
{	
	PFOwnerCharacter = Cast<APFCharacterBase>(GetOwner());

	if (IsValid(PFOwnerCharacter))
	{
		if (APFAICharacter* PFAICharacter = Cast<APFAICharacter>(PFOwnerCharacter))
		{
			if (PFAICharacter->GetBT() != nullptr)
			{
				Blackboard->InitializeBlackboard(*(PFAICharacter->GetBT())->BlackboardAsset);
				BehaviorTreeComponent->StartTree(*(PFAICharacter->GetBT()));
			}
		}
	}
}

void APFAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	PFOwnerCharacter = Cast<APFCharacterBase>(InPawn);

	if (IsValid(PFOwnerCharacter))
	{
		if (APFAICharacter* PFAICharacter = Cast<APFAICharacter>(PFOwnerCharacter))
		{
			if (PFAICharacter->GetBT() != nullptr)
			{
				Blackboard->InitializeBlackboard(*(PFAICharacter->GetBT())->BlackboardAsset);
				BehaviorTreeComponent->StartTree(*(PFAICharacter->GetBT()));
			}

			TeamId = PFAICharacter->GetGenericTeamId();
		}
	}
}

FGenericTeamId APFAIController::GetGenericTeamId() const
{
	return TeamId;
}

ETeamAttitude::Type APFAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);
	if (!OtherTeamAgent)
	{
		return ETeamAttitude::Neutral;
	}
	
	if (GetGenericTeamId() != OtherTeamAgent->GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Friendly;
}

void APFAIController::OnUpdatePerception(AActor* Actor, FAIStimulus Stimulus)
{
	if (!SightConfig) return;
	
	FAISenseID AISenseID = Stimulus.Type;
	if (AISenseID == SightConfig->GetSenseID())
	{
		if (Stimulus.IsActive())
		{
			Blackboard->SetValueAsObject("TargetCharacter", Actor);	
			Blackboard->SetValueAsBool("bCanSightTarget", true);
		}
		else
		{
			Blackboard->SetValueAsBool("bCanSightTarget", false);	
		}
	}
}
