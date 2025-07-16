#pragma once

#include "CoreMinimal.h"
#include "ResourceDelegates.generated.h"

USTRUCT()
struct FResourceDelegatesDummyStruct
{
	GENERATED_BODY()
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FPFStatusChanged, float, OldValue, float, NewValue, float, MaxValue, AActor*, Instigator);