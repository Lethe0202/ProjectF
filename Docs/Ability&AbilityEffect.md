# 스킬 구성 요소
스킬시스템은 크게 UAbilityComponent, UAbilityBase, UAbilityDataAsset 3가지로 이루어집니다.

### UAbilityComponent
* 캐릭터나 오브젝트에 부착되는 컴포넌트로, 스킬을 관리하고 실행하는 역할을 합니다.
* 각 스킬의 활성화, 쿨다운 등을 담당하며, 실제 게임 플레이에서 스킬을 제어하는 중심 컴포넌트입니다.

### UAbilityBase
* 스킬 하나하나를 정의하는 기본 클래스입니다.
* 스킬의 동작, 조건 등을 구현하며, 다양한 스킬을 상속받아 구체화할 수 있습니다.

### UAbilityDataAsset
* 스킬의 데이터만을 담는 에셋입니다.
* 쿨다운 시간, 데미지 수치, 범위, 효과 등 스킬 조정을 위한 파라미터를 담아 코드와 데이터를 분리하며, 디자이너가 쉽게 조정할 수 있도록 합니다.

스킬은 공통적으로 AbilityDataAsset을 기반으로 수치가 결졍되며, 각 스킬의 고유한 속성과 효과는 DataAsset에 설정된 값들을 통해 정의됩니다.
<img width="1417" height="1030" alt="image" src="https://github.com/user-attachments/assets/4e0c0ff1-086e-4075-933d-a96951a59dbf" />



## 몽타주
효과 타이밍은 주로 애님 몽타주에서 Notify, NotifyState로 설정됩니다.
<img width="1844" height="1037" alt="image" src="https://github.com/user-attachments/assets/1cf64137-2109-441f-b8c2-5a1fc3a50f84" />
