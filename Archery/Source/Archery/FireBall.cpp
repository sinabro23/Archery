// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBall.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"
#include "Enemy.h"
// Sets default values
AFireBall::AFireBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollsion = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollsion->InitSphereRadius(15.f);
	SphereCollsion->SetHiddenInGame(false);
	SetRootComponent(SphereCollsion);

	FireballParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireballParticle"));
	FireballParticle->SetupAttachment(GetRootComponent());

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_FIREBALL(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/P_FireBall_Strong.P_FireBall_Strong'"));
	if (PS_FIREBALL.Succeeded())
	{
		FireballParticle->SetTemplate(PS_FIREBALL.Object);
	}


	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_FIREBALLNOHIT(TEXT("ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Primary/FX/P_Gideon_Primary_NoHit.P_Gideon_Primary_NoHit'"));
	if (PS_FIREBALLNOHIT.Succeeded())
	{
		NoImpactParticle = PS_FIREBALLNOHIT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_FIREBALLHIT(TEXT("ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Primary/FX/P_Gideon_Primary_HitWorld.P_Gideon_Primary_HitWorld'"));
	if (PS_FIREBALLHIT.Succeeded())
	{
		ImpactParticle = PS_FIREBALLHIT.Object;
	}

}

// Called when the game starts or when spawned
void AFireBall::BeginPlay()
{
	Super::BeginPlay();
}

void AFireBall::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereCollsion->OnComponentBeginOverlap.AddDynamic(this, &AFireBall::OnBeginOverlap);
}

// Called every frame
void AFireBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RemainTime += DeltaTime;

	SendFireball(DeltaTime);

	if (RemainTime > 3.f)
	{
		if (NoImpactParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NoImpactParticle, GetActorTransform());
		}
		Destroy();
	}

	if (IsHit)
		Destroy();
}

void AFireBall::StartFireBall(const FVector& Direction, AMainCharacter* CauserCharacter)
{
	FireballDirection = Direction;
	MainCharacter = CauserCharacter;
}

void AFireBall::SendFireball(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation() += FireballDirection * Speed * DeltaTime;

	SetActorLocation(CurrentLocation);
}

void AFireBall::SendForward(float DeltaTime)
{
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime);
}

void AFireBall::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy)
	{
		if (ImpactParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, GetActorLocation());
			if (MainCharacter)
			{
				Enemy->OnAttacked(FireballDamage, MainCharacter);
			}
			
		}

		IsHit = true;
	}
	

}

