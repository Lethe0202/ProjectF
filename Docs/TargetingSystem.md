# 타겟팅 시스템 (Targeting System)
### 적 타겟 고정
* 플레이어는 주변의 적 캐릭터 중 하나를 타겟으로 지정할 수 있으며, 타겟이 설정되면 카메라가 해당 적에게 고정됩니다.
* 타겟된 적은 하얀색 빛 효과가 캐릭터 중앙에 표시됩니다.
<img width="1982" height="1155" alt="image" src="https://github.com/user-attachments/assets/d6b861d5-2f0c-4c34-b99b-cacaa39b9272" />

### 타겟 전환
* Q 및 E 키를 사용하여 현재 타겟을 기준으로 좌우에 위치한 다른 적으로 타겟을 전환할 수 있습니다.

### 카메라 자동 조정
* 타겟과 플레이어 사이의 거리가 멀어질 경우, 두 대상이 동시에 화면에 보이도록 카메라 위치와 줌을 자동으로 조정합니다.
<img width="2000" height="1169" alt="image" src="https://github.com/user-attachments/assets/09ceec37-afae-4f56-83f1-21cd771a8de3" />

### UTargetingComponent::UpdateLockOnCamera
#### 단순히 타겟 대상을 보도록 회전하게되면 타겟의 거리에 따라 시점이 매우 불편해지는 현상을 겪었습니다.
#### 때문에 타겟을 대상으로 기본적인 회전 이후에 로컬 오프셋 및 로컬 회전을 추가로 적용했습니다.
```
UTargetingComponent::UpdateLockOnCamera()
{
    ...

	// 목표 방향
	FRotator LookAtRotation = (TargetLocation - OwnerLocation).Rotation();
	
	// 현재 카메라 회전
	FRotator CurrentRotation = OwnerController->GetControlRotation();
	
	// 부드럽게 회전 보간
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, LookAtRotation, GetWorld()->GetDeltaSeconds(), CameraInterpSpeed);

	// 카메라 회전 clamp
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch , -20.f, 20.f);
	
	// 카메라 회전 적용
	OwnerController->SetControlRotation(NewRotation);

	//대상과 플레이어가 원활하게 보이도록 카메라 오프셋 조정 및 로컬 회전
	float Offset = FMath::Clamp(DistanceToTarget / 10.f, 100.f ,DistanceToTarget / 10.f);
	OwnerCameraSpringArmComponent->SocketOffset = FVector(-Offset, 0, Offset * 2.5f);
	
	float PitchRotation = FMath::Max(-1 * DistanceToTarget / 100.f, -180);
	float FinalPitchRotation = FMath::Min(-20.f, PitchRotation);
	OwnerCameraComponent->SetRelativeRotation(FRotator(FinalPitchRotation, 0.f, 0.f));

    ...
}
```
