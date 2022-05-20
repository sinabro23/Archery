// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingSpot.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MainCharacter.h"

// Sets default values
AHealingSpot::AHealingSpot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetSphereRadius(170.f);
	SetRootComponent(SphereComponent);

	HealingSpotParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SpotParticle"));
	HealingSpotParticle->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_HEALINGSPOT(TEXT("ParticleSystem'/Game/RPGEffects/Particles/P_Priest_Aura.P_Priest_Aura'"));
	if (PS_HEALINGSPOT.Succeeded())
	{
		HealingSpotParticle->SetTemplate(PS_HEALINGSPOT.Object);
	}

	HealingSpotParticle->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));

	HealingEffectPaticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HealingEffectPaticle"));
	HealingEffectPaticle->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_HEALINGEFFECT(TEXT("ParticleSystem'/Game/RPGEffects/Particles/P_Priest_Heal_Over_Time_2D.P_Priest_Heal_Over_Time_2D'"));
	if (PS_HEALINGEFFECT.Succeeded())
	{
		HealingEffectPaticle->SetTemplate(PS_HEALINGEFFECT.Object);
	}

	HealingEffectPaticle->SetRelativeLocation(FVector(0.0f, 0.0f, 170.f));

}

// Called when the game starts or when spawned
void AHealingSpot::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHealingSpot::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AHealingSpot::OnSphereBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AHealingSpot::OnSphereEndOverlap);
}

// Called every frame
void AHealingSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealingSpot::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr)
		return;

	auto MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter)
	{
		MainCharacter->OnHealingSpot();
		UE_LOG(LogTemp, Warning, TEXT("Overlapped"));
	}
}

void AHealingSpot::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr)
		return;

	auto MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter)
	{
		MainCharacter->OutHealingSpot();
		UE_LOG(LogTemp, Warning, TEXT("End Overlap"));
	}
}

