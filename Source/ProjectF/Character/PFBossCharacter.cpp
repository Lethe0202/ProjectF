// Fill out your copyright notice in the Description page of Project Settings.


#include "PFBossCharacter.h"

#include "LevelSequencePlayer.h"
#include "Component/AbilityComponent.h"
#include "ProjectF/Ability/AbilityBase.h"
#include "ProjectF/Controller/PFAIController.h"


class ALevelSequenceActor;

APFBossCharacter::APFBossCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APFBossCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APFBossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float APFBossCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float TakeDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (TakeDamageAmount != 0.f)
	{
		if (BossPhaseDataAsset)
		{
			ALevelSequenceActor* SequenceActor;
			FMovieSceneSequencePlaybackSettings SceneSequencePlaybackSettings;
			LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), BossPhaseDataAsset->PhaseChangeSequence, BossPhaseDataAsset->SceneSequencePlaybackSettings, SequenceActor);
			
			LevelSequencePlayer->Play();
			LevelSequencePlayer->OnFinished.AddDynamic(this, &APFBossCharacter::OnChangedPhase);
		}
	}
	
	return TakeDamageAmount;
}

void APFBossCharacter::OnChangedPhase()
{
	APFAIController* PFAICOntroller = Cast<APFAIController>(GetController());

	GetMesh()->GetAnimInstance()->StopAllMontages(0.f);

	AbilityComponent->ClearAbility();
	TArray<TSubclassOf<UAbilityBase>>& NewAbilityClassis = BossPhaseDataAsset->AbilitiesClass;
	
	for (auto NewAbilityClass : NewAbilityClassis)
	{
		auto NewAbility = NewObject<UAbilityBase>(this, NewAbilityClass);
		AbilityComponent->RegisterAbility(NewAbility);
	}
	
	if (PFAICOntroller)
	{
		BehaviorTree = BossPhaseDataAsset->BehaviorTree;
	}
	
	if (LevelSequencePlayer != nullptr)
	{
		LevelSequencePlayer->OnFinished.Clear();
	}
}


