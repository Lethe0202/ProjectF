// Fill out your copyright notice in the Description page of Project Settings.


#include "EQSContext_Target.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UEQSContext_Target::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	UObject* Owner = QueryInstance.Owner.Get();
	if (!Owner) return;
	
	APawn* OwnerPawn = Cast<APawn>(Owner);
	if (!OwnerPawn) return;
	
	AAIController* AIController = Cast<AAIController>(OwnerPawn->GetController());
	if (!AIController) return;

	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	if (!BlackboardComponent) return;
	
	UObject* KeyValue = BlackboardComponent->GetValueAsObject(BlackboardKeyName);
	if (AActor* Actor = Cast<AActor>(KeyValue))
	{
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, Actor);
	}
}
