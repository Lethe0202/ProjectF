// Fill out your copyright notice in the Description page of Project Settings.


#include "PFFunctionLibrary.h"

UWorld* UPFFunctionLibrary::GetActorWorld(const AActor* Actor)
{
	if (!Actor)
	{
		return nullptr;
	}
	
	return Actor->GetWorld();
}

bool UPFFunctionLibrary::IsInGame(const UWorld* World)
{
	if (World)
	{
		return !World->bIsTearingDown;
		//return World->IsGameWorld();
	}
	
	return false;
}
