// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"


enum class ECharacterState : uint8
{
	ECS_Normal, 
	ECS_Cast,
	ECS_Shield,
	ECS_MAX 
};

UENUM(BlueprintType)
enum class ECharacterSkill : uint8
{
	ECS_Meteor = 0,
	ECS_Burden,
	ECS_BlackHole,
	ECS_FireShield,

	ECS_MAX
};

UENUM(BlueprintType)
enum class ECharacterItem : uint8
{
	ECI_HPPotion,
	ECI_MPPotion,

	ECI_MAX
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	ECharacterItem CurrentItem = ECharacterItem::ECI_HPPotion;

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
	UParticleSystem* BlackholeCastParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BlackholeUltimateParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* SkillRangeParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* BurdenMontage;

	class UMainAnimInstance* MainAnim;

	FVector MeteorPosition = FVector();
	FVector BlackholePosition = FVector();

	bool IsEKeyPressed = false;
	bool IsBlackholeKeyPressed = false;
	bool IsShieldOn = false;
	
	float ESkillRange = 3000.f;

	bool bIsAttacking = false;
	bool bIsBurden = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Casting", meta = (AllowPrivateAccess = "true"))
	bool bIsCasting = false;

	class AMainPlayerController* MainPlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Casting", meta = (AllowPrivateAccess = "true"))
	float ESkillCastingTime = 1.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Casting", meta = (AllowPrivateAccess = "true"))
	float BlackholeSkillCastingTime = 3.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Casting", meta = (AllowPrivateAccess = "true"))
	float CurrentCastingTime = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Casting", meta = (AllowPrivateAccess = "true"))
	float CurrentSkillMaxCastingTime = 0.f;
	
	FTimerHandle ESkillTimer;
	FTimerHandle BlackHoleSkillTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Casting", meta = (AllowPrivateAccess = "true"))
	FName CurrentSkillName = FName("Meteor");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Casting", meta = (AllowPrivateAccess = "true"))
	bool IsCurrentSkillAvailable = true;

	// 움직임 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float BaseMovementSpeed = 650.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float CastingMovementSpeed = 400.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float ShieldMovementSpeed = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	ECharacterSkill CurrentSkill = ECharacterSkill::ECS_Meteor;


	///
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UTexture2D* FireballTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UTexture2D* MeteorTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UTexture2D* BurdenTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UTexture2D* BlackholeTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UTexture2D* FireShieldTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UTexture2D* HPPotionTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UTexture2D* MPPotionTexture;
	///

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float CurrentHP = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float MaxHP = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float CurrentMP = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float MaxMP = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float MeteorDamage = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float BlackholeDamage = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	int32 HPPotionCount = 5;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	int32 MPPotionCount = 5;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	int32 CurrentCoinCount = 0;

	UPROPERTY(VisibleAnywhere)
	bool IsDrinkingHPPotion = false;
	UPROPERTY(VisibleAnywhere)
	bool IsDrinkingMPPotion = false;

	FTimerHandle HPPotionTimer;
	FTimerHandle MPPotionTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float HPPotionHealAmount = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float MPPotionHealAmount = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float MeteorMPAMount = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float FireballMPAMount = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float BlackholeMPAMount = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* FireShieldParticle = nullptr;

	FTimerHandle MeteorTimer;
	float MeteorTime = 0.3f;
	int32 MeteorCount = 0;

	bool IsTapOn = false;

	bool IsOnGate = false;

	class ARoomGate* GateRoom = nullptr;

public:
	void MoveForward(float Value);
	void MoveRight(float Value);

	// 마우스 회전함수
	void Turn(float Value);
	void LookUp(float Value);

	void FireWeapon();

	void MeteorKeyPressed();
	void MeteorKeyReleased();
	void MeteorSkillTrail();

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

	void BurdenButtonPressed();

	void BlackHoleSkillPressed();
	void BlackholeTrail();

	void BlackholeKeyReleased();
	void SendBlackhole();
	void BlackholeAttackCheck();

	void SetHP(float NewHP);
	float GetCurrentHP();

	void SetMP(float NewMP);
	float GetCurrentMP();
	
	void TakeHPPotion();
	void TakeMPPotion();
	void TakeCoin(int32 CoinAmount);

	void DrinkHPPotion();
	void DrinkMPPtion();

	void OneKeyPressed();
	void TwoKeyPressed();
	void ThreeKeyPressed();
	void FourKeyPressed();

	void QKeyPressed();

	void EkeyPressed();

	void EndHPHealing();
	void EndMPHealing();

	void FireShieldOn();
	void FireShieldOff();

	void MeteorRepeat();

	void TapKeyPressed();

	void CurrentMPCheck();

	void OnHealingSpot();
	void OutHealingSpot();

	void SetIsOnGate(bool bIsOn);

	void FKeyPressed();

	void SetRoomGate(ARoomGate* RoomGate);
public:

	FORCEINLINE FVector GetBlackholePosition() { return BlackholePosition; };

};
