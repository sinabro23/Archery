// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"


enum class ECharacterState : uint8
{
	ECS_Normal, 
	ECS_Cast,

	ECS_MAX 
};

UCLASS()
class ARCHERY_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

private:

	ECharacterState CharacterState = ECharacterState::ECS_Normal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	//카메라 회전 관련
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate = 45.f; // 초당 어느정도로 카메라가 돌아갈지

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate = 45.f;

	// 조준할때 안할때의 키보드방향키 감도 바꾸게하기위해(마우스x)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float HipTurnRate = 90.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float HipLookUpRate = 90.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float AimingTurnRate = 20.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float AimingLookUpRate = 20.f;
	////////////////////////////////////////////
	// 
	// 마우스 감도 관련
	// meta = (ClampMin = "0.0", ClampMAX = "1.0", UIMin = "1.0", UIMax = "1.0")는 에디터에서 최대값 최소값 한계시켜줌, 디테일창 스크롤하는것의 한계값도 정하기위해 UIMin = "1.0", UIMax = "1.0"
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMAX = "1.0", UIMin = "1.0", UIMax = "1.0"))
	float MouseHipTurnRate = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMAX = "1.0", UIMin = "1.0", UIMax = "1.0"))
	float MouseHipLookUpRate = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMAX = "1.0", UIMin = "1.0", UIMax = "1.0"))
	float MouseAimingTurnRate = 0.2f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMAX = "1.0", UIMin = "1.0", UIMax = "1.0"))
	float MouseAimingLookUpRate = 0.2f;
	///////////////////////////////////////////////

	/** Flash spawned at BarrelSocket */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* SkillRangeParticle;

	/** Montage for firing the weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* AttackMontage;

	class UMainAnimInstance* MainAnim;

	bool IsEKeyPressed = false;
	float ESkillRange = 3000.f;
	

	bool bIsAttacking = false;

public:
	void MoveForward(float Value);
	void MoveRight(float Value);

	// 마우스 회전함수
	void Turn(float Value);
	void LookUp(float Value);

	void FireWeapon();

	void EKeyPressed();
	void EKeyReleased();
	void ESkillTrail();

	UFUNCTION()
	void SendFireBall();

	void AttackEnd();
};
