// Fill out your copyright notice in the Description page of Project Settings.


#include "Burden.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABurden::ABurden()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollsion = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollsion->SetBoxExtent(FVector(16.f, 16.f, 16.f));
	BoxCollsion->SetHiddenInGame(false);
	SetRootComponent(BoxCollsion);

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
void ABurden::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABurden::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void ABurden::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABurden::StartBurden(const FVector& Direction)
{
}

void ABurden::SendBurden(float DeltaTime)
{
}

void ABurden::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

