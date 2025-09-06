#pragma once

#include "ResourceTypes.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStatusTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	float MaxHealth = 0.0f;
	
	UPROPERTY(EditAnywhere)
	float MaxStamina = 0.0f;
};


