#include "ResourceManager.h"

const FCharacterStatusTableRow* UResourceManager::GetCharacterStatusTableRow(FName CharacterID) const
{
	FCharacterStatusTableRow* CharacterStatusTableRow = CharacterStatusDatatable->FindRow<FCharacterStatusTableRow>(CharacterID, "");

	return CharacterStatusTableRow;
}
