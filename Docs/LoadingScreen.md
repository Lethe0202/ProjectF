# 로딩스크린 (Loading Screen)
## 개요
에셋매니저를 활용하여 PrimaryAsset등 게임에 필요한 에셋을 로딩스크린에서 미리 로드할 수 있는 기능입니다.

## 인게임 로딩 스크린
레벨을 이동할 때마다 중간에 로딩 씬을 거쳐 필요한 에셋을 불러오고, 진행률을 표시했습니다.
<img width="1008" height="526" alt="LoadingScren" src="https://github.com/user-attachments/assets/2b2ee4e9-6ec6-4797-9db4-8159943d2692" />

## 에셋 매니저
미리 로드되는 에셋은 프로젝트의 애셋 매니저에 등록된 에셋을 기준으로 불러옵니다.
현재 프로젝트에서는 커스텀 Primary Asset 타입인 AbilityData와 함께 Map, FX 에셋을 미리 로드했습니다.
<img width="1122" height="358" alt="스크린샷 2025-09-06 183524" src="https://github.com/user-attachments/assets/c81a7ea8-67e1-4e11-b56e-80388b76a1f2" />

#### 핵심 사용 함수
````
GetPrimaryAssetDataList
LoadPrimaryAssets
````

## 시행착오 및 해결과정
에셋 매니저에서 에셋 로드가 에디터와 빌드 파일에서는 정상 작동하지만, 독립형 게임(Standalone Game)에서는 동작하지 않는 현상을 경험했습니다. 이는 Standalone 실행이 에디터 모드와 달리 제한된 쿠킹 설정을 사용하기 때문입니다. 빌드 파일은 패키징 과정에서 지정된 에셋이 모두 포함되지만, Standalone 실행은 명확히 참조되지 않은 에셋이 쿠킹에서 제외되어 로드할 수 없게 됩니다.

#### AssetManager Function
````
RegisterPrimaryAssetType
GetAssetsByClass
````

에디터, 독립형 게임, 빌드 게임 3가지 실행을 위와 같은 에셋매니저의 API를 사용하여 차이를 확인했고,
독립형 게임 실행에서는 원활하게 로딩스크린을 테스트할 수 없다는 사실을 확인했습니다.