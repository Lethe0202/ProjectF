// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Step.h"

#include "AIController.h"
#include "ProjectF/Character/PFCharacterBase.h"
#include "ProjectF/Manager/AnimManager.h"
#include "ProjectF/Manager/PFGameInstance.h"

EBTNodeResult::Type UBTTask_Step::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	APFCharacterBase* PFCharacter = OwnerController ? Cast<APFCharacterBase>(OwnerController->GetPawn()) : nullptr;
	if (!PFCharacter) return EBTNodeResult::Failed;
	
	UGameInstance* GameInstance = OwnerComp.GetWorld()->GetGameInstance();
	if (!GameInstance) return EBTNodeResult::Failed;

	UPFGameInstance* PFGameInstance = Cast<UPFGameInstance>(GameInstance);
	if (!PFGameInstance) return EBTNodeResult::Failed;

	UAnimManager* AnimManager = PFGameInstance->GetAnimManager();
	if (!AnimManager) return EBTNodeResult::Failed;
	
	UAnimMontage* RollMontage = AnimManager->GetRollRandomDirection(PFCharacter->GetID());
	if (!RollMontage) return EBTNodeResult::Failed;

	PFCharacter->GetMesh()->GetAnimInstance()->Montage_Play(RollMontage);

	return EBTNodeResult::Succeeded;
}
