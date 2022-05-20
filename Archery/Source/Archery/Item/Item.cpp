// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(GetRootComponent());
	SphereComp->SetSphereRadius(80.f);

	ItemParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ItemParticle"));
	ItemParticle->SetupAttachment(GetRootComponent());

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_PICKUPGLOW(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Treasure/Loot/P_Loot_Idle_Loop.P_Loot_Idle_Loop'"));
	if (PS_PICKUPGLOW.Succeeded())
	{
		ItemParticle->SetTemplate(PS_PICKUPGLOW.Object);
	}

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItem::UseItem(AMainCharacter* CharacterToUse)
{
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

