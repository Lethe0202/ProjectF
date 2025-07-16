#pragma once

#include "CoreMinimal.h"
#include "EffectTypes.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "Engine/DataTable.h"
#include "TargetTypes.h"
#include "ProjectF/Ability/DataAsset/AbilityUIDataAsset.h"
#include "CombatTypes.generated.h"

class UBehaviorTree;
class UAbilityBase;
class ULevelSequence;

UENUM(BlueprintType)
enum class ECharacterDirection : uint8
{
	Forward,
	Backward,
	Left,
	Right
};

UENUM(BlueprintType)
enum class EDynamicCameraType : uint8
{
	Default,
	Near,
	Face,
	Counter,
};

UENUM(BlueprintType)
enum class EActivateCollisionType : uint8
{
	MainWeapon,
	SubWeapon,
};

USTRUCT(BlueprintType)
struct FCombatAnimTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> WeaponEquip;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> WeaponUnEquip;
	
	UPROPERTY(EditAnywhere)
	TMap<ECharacterDirection, TObjectPtr<UAnimMontage>> HitDirectionAnim;
	
	UPROPERTY(EditAnywhere)
	TMap<ECharacterDirection, TObjectPtr<UAnimMontage>> RollDirectionAnim;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> HitInAir;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> Recover;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> Stun;
};

USTRUCT(BlueprintType)
struct FAbilityEffectContainer
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TObjectPtr<UTargetType> TargetType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<TObjectPtr<UEffectType>> Effect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bStrongEffect;
};

UCLASS()
class UAbilityDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAbilityBase> Ability;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UAnimMontage>> AbilityAnim;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAbilityEffectContainer> EffectContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanUseInAir = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHolding = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Holding", meta = (EditCondition = "bHolding", EditConditionHides))
	float MaxHoldTime = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Holding", meta = (EditCondition = "bHolding", EditConditionHides))
	float MinPerfectTiming = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Holding", meta = (EditCondition = "bHolding", EditConditionHides))
	float MaxPerfectTiming = 0.f;
	
	// 카운터 전용 옵션
	UPROPERTY(EditAnywhere)
	bool bIsCounterSkill;

	UPROPERTY(EditAnywhere, meta=(EditCondition="bIsCounterSkill", EditConditionHides))
	TObjectPtr<UAnimMontage> CounterSuccessAnim;

	UPROPERTY(EditAnywhere, Category= "DataAsset")
	TObjectPtr<UAbilityUIDataAsset> AbilityUIDataAsset;
};

USTRUCT()
struct FAbilityInputData
{
	GENERATED_BODY()

public:
	FAbilityInputData()
		: StartHoldTime(0.f),
		CurrentHoldingTime(0.f)
	{
	}
	
public:
	float StartHoldTime;
	float CurrentHoldingTime;
};

UCLASS()
class UBossPhaseDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PhaseHPPercent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UAbilityBase>> AbilitiesClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<ULevelSequence> PhaseChangeSequence;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FMovieSceneSequencePlaybackSettings SceneSequencePlaybackSettings;
};