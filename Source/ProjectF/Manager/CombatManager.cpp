// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatManager.h"

#include "Kismet/GameplayStatics.h"

void UCombatManager::StrongAttackVisualEffect(AActor* Instigator)
{
	if (Instigator && AIStrongAttackParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAttached(AIStrongAttackParticleSystem, Instigator->GetRootComponent());	
	}
}
