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
	// ĳ���Ͱ� �ܳ��ϴ� ������ �����̼�(���콺 ȸ���� ����, Yaw ���� �ð�������� ������ �þ)
	FRotator AimingRotation = MainCharacter->GetBaseAimRotation();
	// ĳ���� �����Ӱ� ���õ� �����̼ǰ�
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(MainCharacter->GetVelocity());

	// ȭ���� ���� ���ϵ� ĳ������ �����Ӹ��� ���� ��. (�������ΰ��� 0, �������ΰ��� 90, �ڷΰ��� -180, �������ΰ��� -80�� �������)
	MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimingRotation).Yaw;

	// ���⶧�� �ִϸ��̼��� ����Ǳ� ���� ĳ������ �������� ���������� ������������ ���� ����.
	if (MainCharacter->GetVelocity().Size() > 0.f)
	{
		LastMovementOffsetYaw = MovementOffsetYaw;
	}
}
