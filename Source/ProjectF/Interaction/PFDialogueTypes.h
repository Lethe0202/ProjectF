// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PFDialogueTypes.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EDialogueNextType : uint8
{
	NextLine		UMETA(DisplayName = "Next Line"),       // 다음 순서 대사
	Choices			UMETA(DisplayName = "Show Choices"),    // 선택지 보여주기
	EndDialogue		UMETA(DisplayName = "End Dialogue"),	// 대화 종료
	//TriggerEvent	UMETA(DisplayName = "Trigger Event")	// 이벤트 발생 후 다음으로
};

USTRUCT(BlueprintType, Blueprintable)
struct FDialogueChoiceInfo
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	EDialogueNextType DialogueNextType;

	UPROPERTY(EditAnywhere)
	FText ChoiceDisplayText;
	
	UPROPERTY(EditAnywhere)
	FName NextDialogueID;
};

USTRUCT(BlueprintType, Blueprintable)
struct FDialogueTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FText DisplayName;
	
	UPROPERTY(EditAnywhere)
	FText Dialogue;
	
	UPROPERTY(EditAnywhere)
	EDialogueNextType DialogueNextType;
	
	UPROPERTY(EditAnywhere, meta = (EditCondition = "DialogueNextType == EDialogueNextType::NextLine", hidden))
	FName NextDialogueID;
	
	UPROPERTY(EditAnywhere, meta = (EditCondition = "DialogueNextType == EDialogueNextType::Choices", hidden))
	TArray<FDialogueChoiceInfo> DialogueChoices;
};
