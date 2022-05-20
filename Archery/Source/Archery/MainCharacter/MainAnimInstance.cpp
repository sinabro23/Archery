// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMainAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (MainCharacter == nullptr)
	{
		MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
	}

	if (MainCharacter)
	{
		FVector Velocity = MainCharacter->GetVelocity();
		Velocity.Z = 0;
		Speed = Velocity.Size();

		bIsInAir = MainCharacter->GetCharacterMovement()->IsFalling();

		if (MainCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
		{
			bIsAccelerating = true;
		}
		else
		{
			bIsAccelerating = false;
		}

		CalculateMovementOffset();
	
	}

}

void UMainAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
}

void UMainAnimInstance::AnimNotify_SendFireBall()
{
	OnSendFireBall.Broadcast();
}

void UMainAnimInstance::AnimNotify_FireballAttackEnd()
{
	OnFireBallEnd.Broadcast();
}

void UMainAnimInstance::AnimNotify_CastingEnd()
{
	OnCastingEnd.Broadcast();
}

void UMainAnimInstance::AnimNotify_BurdenEnd()
{
	OnBurdenEnd.Broadcast();
}

void UMainAnimInstance::AnimNotify_BurdenFire()
{
	OnBurdenFire.Broadcast();
}

void UMainAnimInstance::CalculateMovementOffset()
{
	// 캐릭터가 겨냥하는 방향의 로테이션(마우스 회전과 관련, Yaw 값이 시계방향으로 갈수록 늘어남)
	FRotator AimingRotation = MainCharacter->GetBaseAimRotation();
	// 캐릭터 움직임과 관련된 로테이션값
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(MainCharacter->GetVelocity());

	// 화면을 어디로 향하든 캐릭터의 움직임만을 위한 값. (정면으로가면 0, 우측으로가면 90, 뒤로가면 -180, 좌측으로가면 -80에 가까워짐)
	MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimingRotation).Yaw;

	// 멈출때의 애니메이션이 적용되기 위해 캐릭터의 움직임이 있을때에만 지난프레임의 값을 저장.
	if (MainCharacter->GetVelocity().Size() > 0.f)
	{
		LastMovementOffsetYaw = MovementOffsetYaw;
	}
}
