// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomGate.h"
#include "Components/SphereComponent.h"
#include "MainCharacter.h"

// Sets default values
ARoomGate::ARoomGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GATEMESH(TEXT("StaticMesh'/Game/ParagonProps/Monolith/Dawn/Meshes/Dawn_Gate_Orb.Dawn_Gate_Orb'"));
	if (SM_GATEMESH.Succeeded())
	{
		GateMesh->SetStaticMesh(SM_GATEMESH.Object);
	}
	SetRootComponent(GateMesh);

	WidgetSphere = CreateDefaultSubobject<USphereComponent>(TEXT("WidgetSphere"));
	WidgetSphere->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ARoomGate::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoomGate::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	WidgetSphere->OnComponentBeginOverlap.AddDynamic(this, &ARoomGate::OnSphereBeginOverlap);
	WidgetSphere->OnComponentEndOverlap.AddDynamic(this, &ARoomGate::OnSphereEndOverlap);
}

// Called every frame
void ARoomGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoomGate::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr)
		return;

	auto MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter)
	{
		IsCharacterOn = true;
		UE_LOG(LogTemp, Warning, TEXT("Overlapped"));
		
		ShowWidget();
	}
	
}

void ARoomGate::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr)
		return;

	auto MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter)
	{
		IsCharacterOn = false;
		UE_LOG(LogTemp, Warning, TEXT("End Overlap"));

		HideWidget();
	}
}

