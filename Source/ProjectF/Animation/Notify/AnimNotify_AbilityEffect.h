// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AbilityEffect.generated.h"

class UAbilityBase;

UENUM(BlueprintType)
enum class EActionEventType : uint8
{
	WeaponAttack,   // 무기 판정용 콜리전
	Ability,		// 연출용 타이밍 이벤트 (투사체생성 등)
	//Collision		// 별도의 Trace 판정 (필드 상 오브젝트용)
};

UENUM(BlueprintType)
enum class EShape : uint8
{
	Box				UMETA(DisplayName = "Box"),
	Sphere			UMETA(DisplayName = "Sphere"),
	Capsule			UMETA(DisplayName = "Capsule"),
};

USTRUCT(BlueprintType)
struct FAttackCollisionShape
{
	GENERATED_USTRUCT_BODY()

	FAttackCollisionShape()
	{
		ShapeType = EShape::Box;
	};
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShape", meta = (EditCondition = "ShapeType == EShape::Box"))
	float HalfExtentX = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShape", meta = (EditCondition = "ShapeType == EShape::Box"))
	float HalfExtentY = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShape", meta = (EditCondition = "ShapeType == EShape::Box"))
	float HalfExtentZ = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShape", meta = (EditCondition = "ShapeType == EShape::Sphere"))
	float SphereRadius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShape", meta = (EditCondition = "ShapeType == EShape::Capsule"))
	float CapsuleRadius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShape", meta = (EditCondition = "ShapeType == EShape::Capsule"))
	float HalfHeight = 0.f;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShape")
	EShape ShapeType;
};

/**
 * 
 */
UCLASS()
class PROJECTF_API UAnimNotify_AbilityEffect : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
	UPROPERTY(EditAnywhere, Category="AbilityInfo")
	EActionEventType ActionEventType;
	
	UPROPERTY(EditAnywhere, Category="AbilityInfo", meta = (EditCondition = "ActionEventType != EActionEventType::WeaponAttack", EditConditionHides))
	TSubclassOf<UAbilityBase> AbilityClass;
	
	UPROPERTY(EditAnywhere, Category="AbilityInfo", meta = (EditCondition = "ActionEventType != EActionEventType::WeaponAttack", EditConditionHides))
	int32 Index;
};
