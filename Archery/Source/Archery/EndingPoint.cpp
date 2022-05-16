// Fill out your copyright notice in the Description page of Project Settings.


#include "EndingPoint.h"
#include "Components/SphereComponent.h"
#include "MainCharacter.h"
#include "MainPlayerController.h"

// Sets default values
AEndingPoint::AEndingPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EndingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("EndingSphere"));
	SetRootComponent(EndingSphere);
}

// Called when the game starts or when spawned
void AEndingPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEndingPoint::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	EndingSphere->OnComponentBeginOverlap.AddDynamic(this, &AEndingPoint::OnSphereBeginOverlap);
	EndingSphere->OnComponentEndOverlap.AddDynamic(this, &AEndingPoint::OnSphereEndOverlap);
}

// Called every frame
void AEndingPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEndingPoint::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
		return;

	AMainCharacter* Maincharacter = Cast<AMainCharacter>(OtherActor);
	if (Maincharacter)
	{
		if (Maincharacter->GetMainPlayerController())
		{
			Maincharacter->GetMainPlayerController()->SetPause(true);
			Maincharacter->GetMainPlayerController()->SetEndingHUDVisibility(true);
			Maincharacter->SetIsGameEnd(true);
		}
	}
}

void AEndingPoint::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor)
		return;


}

