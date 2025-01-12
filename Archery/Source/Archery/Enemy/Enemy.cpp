// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"
#include "MainCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "HPPotion.h"
#include "MPPotion.h"
#include "Coin.h"
#include "NavigationSystem.h"

int32 AEnemy::EnemiesCounts = 0;

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());

	// Create the Combat Range Sphere
	CombatRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatRange"));
	CombatRangeSphere->SetupAttachment(GetRootComponent());

	// Construct left and right weapon collision boxes
	LeftWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftWeaponBox"));
	LeftWeaponCollision->SetupAttachment(GetMesh(), FName("LeftWeaponBone"));
	RightWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightWeaponBox"));
	RightWeaponCollision->SetupAttachment(GetMesh(), FName("RightWeaponBone"));

	StunParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("StunParticleSystem"));
	StunParticle->SetupAttachment(GetRootComponent());

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_STUN(TEXT("ParticleSystem'/Game/RPGEffects/Particles/P_Status_Stun.P_Status_Stun'"));
	if (PS_STUN.Succeeded())
	{
		StunParticle->SetTemplate(PS_STUN.Object);
	}

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemiesCounts++;

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	EnemyController = Cast<AEnemyController>(GetController());

	// 패트롤포인트를 월드포지션으로 넣기
	const FVector WorldPatrolPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), PatrolPoint);
	const FVector WorldPatrolPoint2 = UKismetMathLibrary::TransformLocation(GetActorTransform(), PatrolPoint2);
	//DrawDebugSphere(GetWorld(), WorldPatrolPoint2, 25.f, 12, FColor::Red, true);

	EnemyController = Cast<AEnemyController>(GetController());

	if (EnemyController)
	{
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		if (NavSystem)
		{
			FVector Origin = GetActorLocation();
			FNavLocation NextPatrolLocation;
			FNavLocation NextPatrolLocation2;

			if (NavSystem->GetRandomPointInNavigableRadius(Origin, 50.f, NextPatrolLocation))
			{
				PatrolPoint = NextPatrolLocation.Location;
				PatrolPoint.Z = 0.f;
			}

			if (NavSystem->GetRandomPointInNavigableRadius(Origin, 1000.f, NextPatrolLocation2))
			{
				PatrolPoint2 = NextPatrolLocation2.Location;
				PatrolPoint2.Z = 0.f;
			}

			EnemyController->GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolPoint"), NextPatrolLocation);
			EnemyController->GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolPoint2"), NextPatrolLocation2);
		}
		EnemyController->GetBlackboardComponent()->SetValueAsBool(FName("CanAttack"), true);

		EnemyController->RunBehaviorTree(BehaviorTree);
		
	}

	StunParticle->SetActive(false);

	BlackholePosition = GetActorLocation();

	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bStunned)
	{
		StunParticle->SetActive(true);
	}
	else
	{
		StunParticle->SetActive(false);
	}

	DraggedByBlackhole(DeltaTime);
	
	if (EnemiesCounts == 0)
		AnounceEnemiesZero();
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOverlap);

	CombatRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatRangeOverlap);
	CombatRangeSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatRangeEndOverlap);

	// Bind functions to overlap events for weapon boxes
	LeftWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnLeftWeaponOverlap);
	RightWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnRightWeaponOverlap);

	// Set collision presets for weapon boxes
	LeftWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftWeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LeftWeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LeftWeaponCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightWeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RightWeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightWeaponCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AEnemy::OnAttacked(float DamageAmount, AMainCharacter* MainCharacter)
{
	float NewHP = CurrentHP - DamageAmount;
	SetHP(NewHP);

	if (bDying)
		return;

	PlayHitMontage(FName("HitReactFront"));

	const float Stunned = FMath::FRandRange(0.f, 1.f);
	if (Stunned <= StunChance)
	{
		// Stun the Enemy
		PlayHitMontage(FName("HitReactFront"));
		SetStunned(true);
	}

	if (EnemyController)
	{
		EnemyController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), MainCharacter);
	}
}

void AEnemy::OnAttackedBlackhole(float DamageAmount, AMainCharacter* MainCharacter)
{
	if (bDying)
		return;

	PlayHitMontage(FName("HitReactFront"));
	SetStunned(true);
	if (EnemyController)
	{
		EnemyController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), MainCharacter);
	}
	
	GetWorldTimerManager().SetTimer(BlackholeAttackTimer, this, &AEnemy::BlackholeRepeat, BlackholeTime);
	BlackholePosition = MainCharacter->GetBlackholePosition();

	bIsOnBlackhole = true;

	BlackholeDamage = DamageAmount;
}

float AEnemy::GetMaxHP()
{
	return MaxHP;
}

float AEnemy::GetCurrentHP()
{
	return CurrentHP;
}

float AEnemy::GetHPRatio()
{
	return CurrentHP / MaxHP;
}

void AEnemy::SetHP(float NewHP)
{
	CurrentHP = NewHP;

	if (CurrentHP <= 0.f)
	{
		CurrentHP = 0.f;
		Die();
	}
	else
	{
		ShowHealthBar();
	}
		

	OnHPChanged.Broadcast();
}



void AEnemy::ShowHealthBar_Implementation()
{
	GetWorldTimerManager().ClearTimer(HealthBarTimer);
	GetWorldTimerManager().SetTimer(
		HealthBarTimer,
		this,
		&AEnemy::HideHealthBar,
		HealthBarDisplayTime);
}

void AEnemy::Die()
{
	if (bDying)
		return;

	bDying = true;
	bStunned = false;

	EnemiesCounts--;

	SetActorEnableCollision(false);
	HideHealthBar();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage, 1.f);
		AnimInstance->Montage_JumpToSection(FName("Death"), DeathMontage);
	}

	if (EnemyController)
	{
		EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("Dead"), true);
		EnemyController->StopMovement();
	}
}

void AEnemy::PlayHitMontage(FName Section, float PlayRate)
{
	if (bCanHitReact)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(HitMontage, PlayRate);
			AnimInstance->Montage_JumpToSection(Section, HitMontage);
		}

		bCanHitReact = false;
		const float HitReactTime = FMath::FRandRange(HitReactTimeMin, HitReactTimeMax);
		GetWorldTimerManager().SetTimer(HitReactTimer, this, &AEnemy::ResetHitReactTimer, HitReactTime);

	}
}

void AEnemy::ResetHitReactTimer()
{
	bCanHitReact = true;
}

void AEnemy::AgroSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;

	auto Character = Cast<AMainCharacter>(OtherActor);
	if (Character)
	{
		// Set the value of the Target Blackboard Key
		if (EnemyController)
		{
			EnemyController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Character);
		}
		
	}
}

void AEnemy::SetStunned(bool Stunned)
{
	bStunned = Stunned;

	if (EnemyController)
	{
		EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("Stunned"), Stunned);
	}
}

void AEnemy::AnounceEnemiesZero()
{
	OnEnemiesZero.Broadcast();
}

void AEnemy::CombatRangeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;

	auto ShooterCharacter = Cast<AMainCharacter>(OtherActor);
	if (ShooterCharacter)
	{
		bInAttackRange = true;
		if (EnemyController)
		{
			EnemyController->GetBlackboardComponent()->SetValueAsBool(
				TEXT("InAttackRange"),
				true
			);
		}
	}
}


void AEnemy::CombatRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr) return;

	auto ShooterCharacter = Cast<AMainCharacter>(OtherActor);
	if (ShooterCharacter)
	{
		bInAttackRange = false;
		if (EnemyController)
		{
			EnemyController->GetBlackboardComponent()->SetValueAsBool(
				TEXT("InAttackRange"),
				false
			);
		}
	}
}

void AEnemy::PlayAttackMontage(FName Section, float PlayRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(Section, AttackMontage);
	}
	bCanAttack = false;

	GetWorldTimerManager().SetTimer(AttackWaitTimer, this, &AEnemy::ResetCanAttack, AttackWaitTime);

	if (EnemyController)
	{
		EnemyController->GetBlackboardComponent()->SetValueAsBool(FName("CanAttack"), false);
	}
}

FName AEnemy::GetAttackSectionName()
{
	FName SectionName;
	const int32 Section = FMath::RandRange(1, 4);
	switch (Section)
	{
	case 1:
		SectionName = AttackA;
		break;
	case 2:
		SectionName = AttackB;
		break;
	case 3:
		SectionName = AttackC;
		break;
	case 4:
		SectionName = AttackD;
		break;
	}
	return SectionName;
}

void AEnemy::OnLeftWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Character = Cast<AMainCharacter>(OtherActor);
	if (Character)
	{
		DoDamage(Character);
	}
}

void AEnemy::OnRightWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Character = Cast<AMainCharacter>(OtherActor);
	if (Character)
	{
		DoDamage(Character);
	}
}

void AEnemy::ActivateLeftWeapon()
{
	LeftWeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemy::DeactivateLeftWeapon()
{
	LeftWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::ActivateRightWeapon()
{
	RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemy::DeactivateRightWeapon()
{
	RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::DoDamage(AMainCharacter* Victim)
{
	if (Victim == nullptr) return;

	UGameplayStatics::ApplyDamage(Victim, BaseDamage, EnemyController, this, UDamageType::StaticClass());
}

void AEnemy::ResetCanAttack()
{
	bCanAttack = true;
	if (EnemyController)
	{
		EnemyController->GetBlackboardComponent()->SetValueAsBool(FName("CanAttack"), true);
	}
}

void AEnemy::FinishDeath()
{
	GetMesh()->bPauseAnims = true;

	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::DestroyEnemy, DeathTime);
}

void AEnemy::DestroyEnemy()
{
	Destroy();
	DropItem();
}

void AEnemy::BlackholeRepeat()
{
	if (bDying)
	{
		bIsOnBlackhole = false;
		SetHP(0);
		return;
	}

	if (CurrentBlackholeCount > MaxBlackholeCount)
	{
		CurrentBlackholeCount = 0;
		bIsOnBlackhole = false;
	}
	else
	{
		PlayHitMontage(FName("HitReactFront"));
		SetStunned(true);
		CurrentBlackholeCount++;
		GetWorldTimerManager().SetTimer(BlackholeAttackTimer, this, &AEnemy::BlackholeRepeat, BlackholeTime);
		
		float NewHP = CurrentHP - BlackholeDamage;
		SetHP(NewHP);
	}
}

void AEnemy::DraggedByBlackhole(float DeltaTime)
{
	if (bIsOnBlackhole)
	{
		FVector DirectToBlackhole = BlackholePosition - GetActorLocation();
		if (DirectToBlackhole.Normalize())
		{
			SetActorLocation(GetActorLocation() + DirectToBlackhole * DeltaTime * 60.f);
		}
	}
}

bool AEnemy::ShouldHappenInPercent(int32 Percentage)
{
	return (FMath::RandRange(1, 100 / Percentage) == 1 ? true : false);
}

void AEnemy::DropItem()
{
	// 50 퍼센트는 코인을 드랍
	if (ShouldHappenInPercent(50))
	{
		SpawnCoin();
	}
	else
	{
		// 나머지 50퍼센트 드랍은 포션드랍
		if (ShouldHappenInPercent(50))
		{
			// 25퍼센트는 HPPotion 드랍
			SpawnHPPotion();
		}
		else
		{	// 25퍼센트는 MPPotion 드랍
			SpawnMPPotion();
		}
	}
}


void AEnemy::SpawnHPPotion()
{
	FVector DropLocation = GetActorLocation() + (-1.f * GetActorUpVector()) * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	GetWorld()->SpawnActor<AHPPotion>(DropLocation, FRotator::ZeroRotator);
}

void AEnemy::SpawnMPPotion()
{
	FVector DropLocation = GetActorLocation() + (-1.f * GetActorUpVector()) * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	GetWorld()->SpawnActor<AMPPotion>(DropLocation, FRotator::ZeroRotator);
}

void AEnemy::SpawnCoin()
{
	FVector DropLocation = GetActorLocation() + (-1.f * GetActorUpVector()) * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	ACoin* Coin = GetWorld()->SpawnActor<ACoin>(DropLocation + FVector(0.0f, 0.0f, 15.f), FRotator::ZeroRotator);
	int32 CoinAmount = FMath::FRandRange(1000, 1500);
	if (Coin)
	{
		Coin->SetCoinAmount(CoinAmount);
	}
}
