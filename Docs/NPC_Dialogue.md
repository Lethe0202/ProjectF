# NPC 상호작용
## 상호작용 시작
* NPC와 일정 거리 이내일 때만 상호작용 가능하며, 가까이 다가가면 UI 힌트(G 키)가 표시됩니다.
* G 키를 눌러 NPC와 상호작용할 수 있으며, 상호작용 시 대화가 시작됩니다.
* 대화는 DataTable 정보를 기반으로 진행됩니다.
![2025-07-17 002752](https://github.com/user-attachments/assets/232fb41c-b7f2-4050-811d-3093daecba8d)

## 선택지 기반 대화
* 대화 중에는 다양한 선택지가 제공되며, 선택에 따라 대사 흐름이 달라집니다. 특정 선택지를 통해 대화를 종료할 수도 있습니다.
<img width="1958" height="1136" alt="image" src="https://github.com/user-attachments/assets/b3e3119a-a052-4bc5-9020-b2339d50d64b" />

## 위젯과 데이터 구조
UDialogueWidget은 DialogueManager에게 DataTable 기반으로 아래 구조체와 같은 데이터를 요청하고, 받은 정보를 플레이어에게 화면에 표시합니다.

### Interaction/PFDialogueTypes.h
```
USTRUCT(BlueprintType, Blueprintable)
struct FDialogueChoiceInfo
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	EDialogueNextType DialogueNextType = EDialogueNextType::NextLine;

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
	
	UPROPERTY(EditAnywhere, meta = (MultiLine = "true"))
	FText Dialogue;
	
	UPROPERTY(EditAnywhere)
	EDialogueNextType DialogueNextType;
	
	UPROPERTY(EditAnywhere, meta = (EditCondition = "DialogueNextType == EDialogueNextType::NextLine", hidden))
	FName NextDialogueID;
	
	UPROPERTY(EditAnywhere, meta = (EditCondition = "DialogueNextType == EDialogueNextType::Choices", hidden))
	TArray<FDialogueChoiceInfo> DialogueChoices;
};
```
### UDataTable
<img width="1103" height="483" alt="image" src="https://github.com/user-attachments/assets/792640e7-8af4-47f0-a8d2-0aca98fecf64" />
