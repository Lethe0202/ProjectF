#pragma once

#include "CoreMinimal.h"
#include "ProjectF/Common/ResourceTypes.h"
#include "UObject/Object.h"
#include "ResourceManager.generated.h"


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECTF_API UResourceManager : public UObject
{
	GENERATED_BODY()
	
public:
	const FCharacterStatusTableRow* GetCharacterStatusTableRow(FName CharacterID) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DataTable")
	TObjectPtr<UDataTable> CharacterStatusDatatable; 
};
