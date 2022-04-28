// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "MainAnimInstance.h"
#include "FireBall.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "MainPlayerController.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 280.f;
	CameraBoom->bUsePawnControlRotation = true; // 컨트롤러 기준으로 회전한다
	CameraBoom->SocketOffset = FVector(0.f, 60.f, 90.f); // 카메라가 달릴 소켓을 움직임

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // 카메라는 컨트롤러따라 회전하면 안됨.

	// 마우스 돌리면 캐릭터가 회전할지안할지
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;  // Yaw는 컨트롤러에 따라 회전하게
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false; // 인풋 들어오는 방향으로 캐릭터가 회전하지않음
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f); // 이 로테이션레이트로 회전함
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	SkillRangeParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SkillRangeParticle"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_SKILLRANGE(TEXT("ParticleSystem'/Game/RPGEffects/Particles/P_Targeting_Player_Select.P_Targeting_Player_Select'"));
	if (PS_SKILLRANGE.Succeeded())
	{
		SkillRangeParticle->SetTemplate(PS_SKILLRANGE.Object);
	}
	SkillRangeParticle->SetActive(true);
	SkillRangeParticle->SetHiddenInGame(true);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_METEOR(TEXT("ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Meteor/FX/P_Gideon_Meteor_Shower.P_Gideon_Meteor_Shower'"));
	if (PS_METEOR.Succeeded())
	{
		MeteorParticle = PS_METEOR.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_METEORAREA(TEXT("ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Meteor/FX/P_Gideon_Meteor_Portal_Fast.P_Gideon_Meteor_Portal_Fast'"));
	if (PS_METEORAREA.Succeeded())
	{
		MeteorAreaParticle = PS_METEORAREA.Object;
	}
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	MainPlayerController = Cast<AMainPlayerController>(GetController());
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ESkillTrail();
	SetCharacterMovementSpeed();

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::Turn); // 마우스 이동은 함수 구현 안하고 바로
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("EKey", EInputEvent::IE_Pressed, this, &AMainCharacter::EKeyPressed);
	PlayerInputComponent->BindAction("EKey", EInputEvent::IE_Released, this, &AMainCharacter::EKeyReleased);

	PlayerInputComponent->BindAction("FireButton", IE_Pressed, this, &AMainCharacter::FireWeapon);
}

void AMainCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MainAnim = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance());
	if (MainAnim)
	{
		MainAnim->OnSendFireBall.AddUObject(this, &AMainCharacter::SendFireBall);
		MainAnim->OnFireBallEnd.AddUObject(this, &AMainCharacter::AttackEnd);
		//MainAnim->OnCastingEnd.AddUObject(this, &AMainCharacter::SendMeteor);
	}
}

void AMainCharacter::MoveForward(float Value)
{

	if (Controller && Value != 0.f)
	{
		// 컨트롤러기준 정면 구하기
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // X가 정면 방향이니깐
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::MoveRight(float Value)
{

	if (Controller && Value != 0.f)
	{
		// 컨트롤러기준 정면 구하기
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); // Y가 우측 방향이니깐
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::Turn(float Value)
{
	float TurnScaleFactor = 0.0f;
	TurnScaleFactor = MouseHipTurnRate;
	AddControllerYawInput(Value * TurnScaleFactor);
}

void AMainCharacter::LookUp(float Value)
{
	float LookUpScaleFactor = 0.0f;
	LookUpScaleFactor = MouseHipLookUpRate;
	AddControllerPitchInput(Value * LookUpScaleFactor);
}

void AMainCharacter::FireWeapon()
{
	if (bIsAttacking)
		return;

	bIsAttacking = true;

	if (MainAnim && AttackMontage)
	{
		MainAnim->Montage_Play(AttackMontage);
		MainAnim->Montage_JumpToSection(FName("FireBall"));
	}
}

void AMainCharacter::EKeyPressed()
{
	if (bIsAttacking || CharacterState == ECharacterState::ECS_Cast)
		return;

	IsEKeyPressed = true;
}

void AMainCharacter::EKeyReleased()
{
	if (bIsAttacking || !IsEKeyPressed)
		return;

	IsEKeyPressed = false;
	SkillRangeParticle->SetHiddenInGame(true);

	// TODO ESkill 발사
	if (MainAnim && AttackMontage)
	{
		MainAnim->Montage_Play(AttackMontage);
		MainAnim->Montage_JumpToSection(FName("Cast"));
	}

	bIsCasting = true;
	
	CharacterState = ECharacterState::ECS_Cast;
	if (MainPlayerController)
	{
		MainPlayerController->SetWidgetVisiblity(true);
	}

	GetWorldTimerManager().SetTimer(
		ESkillTimer,
		this,
		&AMainCharacter::SendMeteor,
		ESkillCastingTime);
}

void AMainCharacter::ESkillTrail()
{

	if (IsEKeyPressed)
	{

		FVector2D ViewportSize;
		if (GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->GetViewportSize(ViewportSize);
		}

		FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
		FVector CrosshairWorldPosition;
		FVector CrosshairWorldDirection;

		bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
			UGameplayStatics::GetPlayerController(this, 0),
			CrosshairLocation,
			CrosshairWorldPosition,
			CrosshairWorldDirection);

		if (bScreenToWorld) 
		{
			FHitResult ScreenTraceHit;
			const FVector Start = CrosshairWorldPosition;
			const FVector End = CrosshairWorldPosition + CrosshairWorldDirection * ESkillRange;

			FVector BeamEndPoint = End;

			GetWorld()->LineTraceSingleByChannel(
				ScreenTraceHit,
				Start,
				End,
				ECollisionChannel::ECC_Visibility);
			if (ScreenTraceHit.bBlockingHit) // was there a trace hit?
			{
				BeamEndPoint = ScreenTraceHit.Location;
				MeteorPosition = ScreenTraceHit.Location + FVector(0.0f, 0.0f, 1000.f);
				//DrawDebugLine(GetWorld(), Start, BeamEndPoint, FColor::Red, false, 2.f);
				//DrawDebugPoint(GetWorld(), ScreenTraceHit.Location, 5.f, FColor::Red, false, 2.f);
				SkillRangeParticle->SetHiddenInGame(false);
				SkillRangeParticle->SetRelativeLocation(ScreenTraceHit.Location + FVector(0.0f, 0.0f, 10.f));
			}
		}

		
	}
}

void AMainCharacter::SendFireBall()
{
	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket");
	if (BarrelSocket)
	{
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());

		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
		}

		// Get current size of the viewport
		FVector2D ViewportSize;
		if (GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->GetViewportSize(ViewportSize);
		}

		// Get screen space location of crosshairs
		FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
		FVector CrosshairWorldPosition;
		FVector CrosshairWorldDirection;

		// Get world position and direction of crosshairs
		bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
			UGameplayStatics::GetPlayerController(this, 0),
			CrosshairLocation,
			CrosshairWorldPosition,
			CrosshairWorldDirection);

		if (bScreenToWorld) // was deprojection successful?
		{
			FHitResult ScreenTraceHit;
			const FVector Start = CrosshairWorldPosition;
			const FVector End = CrosshairWorldPosition + CrosshairWorldDirection * 50'000.f;

			// Set beam end point to line trace end point
			FVector BeamEndPoint = End;

			// Trace outward from crosshairs world location
			GetWorld()->LineTraceSingleByChannel(
				ScreenTraceHit,
				Start,
				End,
				ECollisionChannel::ECC_Visibility);
			if (ScreenTraceHit.bBlockingHit) // was there a trace hit?
			{
				// Beam end point is now trace hit location
				BeamEndPoint = ScreenTraceHit.Location;
				//DrawDebugLine(GetWorld(), Start, BeamEndPoint, FColor::Red, false, 2.f);
				//DrawDebugPoint(GetWorld(), ScreenTraceHit.Location, 5.f, FColor::Red, false, 2.f);
			}

			AFireBall* Fireball = GetWorld()->SpawnActor<AFireBall>(SocketTransform.GetLocation(), GetActorRotation());
			Fireball->StartFireBall(CrosshairWorldDirection);
		}
	}
}

void AMainCharacter::SendMeteor()
{
	if (MainAnim && AttackMontage)
	{
		MainAnim->Montage_Play(AttackMontage);
		MainAnim->Montage_JumpToSection(FName("Rift"));

		if (MeteorParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MeteorParticle, MeteorPosition);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MeteorAreaParticle, MeteorPosition);
		}
	}

	bIsCasting = false;
	CharacterState = ECharacterState::ECS_Normal;
	if (MainPlayerController)
	{
		MainPlayerController->SetWidgetVisiblity(false);
	}
}

void AMainCharacter::AttackEnd()
{
	bIsAttacking = false;
}

bool AMainCharacter::IsCasting()
{
	return CharacterState == ECharacterState::ECS_Cast;
}

void AMainCharacter::SetCharacterMovementSpeed()
{
	switch (CharacterState)
	{
	case ECharacterState::ECS_Normal:
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
		break;
	case ECharacterState::ECS_Cast:
		GetCharacterMovement()->MaxWalkSpeed = CastingMovementSpeed;
		break;
	case ECharacterState::ECS_MAX:
		break;
	default:
		break;
	}
}

float AMainCharacter::GetESkillRatio()
{
	return 0.0f;
}





