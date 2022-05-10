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

UENUM(BlueprintType)
enum class ECharacterSkill : uint8
{
	ECS_Meteor = 0,
	ECS_Burden,

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

	// Take combat damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

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


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MeteorParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MeteorAreaParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* SkillRangeParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* BurdenMontage;

	class UMainAnimInstance* MainAnim;

	FVector MeteorPosition = FVector();

	bool IsEKeyPressed = false;
	
	float ESkillRange = 3000.f;

	bool bIsAttacking = false;
	bool bIsBurden = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Casting", meta = (AllowPrivateAccess = "true"))
	bool bIsCasting = false;

	class AMainPlayerController* MainPlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Casting", meta = (AllowPrivateAccess = "true"))
	float ESkillCastingTime = 1.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Casting", meta = (AllowPrivateAccess = "true"))
	float CurrentCastingTime = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Casting", meta = (AllowPrivateAccess = "true"))
	float CurrentSkillMaxCastingTime = 0.f;
	
	FTimerHandle ESkillTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Casting", meta = (AllowPrivateAccess = "true"))
	FName CurrentSkillName = FName("None");

	// 움직임 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float BaseMovementSpeed = 650.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float CastingMovementSpeed = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	ECharacterSkill CurrentSkill = ECharacterSkill::ECS_Meteor;


	///
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UTexture2D* FireballTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UTexture2D* MeteorTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UTexture2D* BurdenTexture;
	///

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float CurrentHP = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float MaxHP = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float CurrentMP = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float MaxMP = 100.f;

	float MeteorDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathMontage;

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

	void RMBButtonPressed();
	void RMBButtonReleased();

	UFUNCTION()
	void SendFireBall();

	UFUNCTION()
	void SendMeteor();

	UFUNCTION()
	void SendBurden();

	void AttackEnd();
	void BurdenEnd();

	UFUNCTION(BlueprintCallable)
	bool IsCasting();

	void SetCharacterMovementSpeed();

	float GetESkillRatio();

	void SkillChange();

	void MeteorAttackCheck();

	void Die();

	UFUNCTION(BlueprintCallable)
	void FinishDeath();
};
