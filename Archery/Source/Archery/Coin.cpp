// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"
#include "MainCharacter.h"	
#include "Components/SphereComponent.h"

ACoin::ACoin()
{
	ItemName = FName("Coin");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Coin(TEXT("StaticMesh'/Game/ExampleContent/Blueprints/Meshes/SM_Pickup_Coin.SM_Pickup_Coin'"));
	if (SM_Coin.Succeeded())
	{
		ItemMesh->SetStaticMesh(SM_Coin.Object);
	}

	ItemMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5));
}

void ACoin::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACoin::OnBeginOverlap);
}

void ACoin::Tick(float DeltaTime)
{
	FRotator Rotation = GetActorRotation();
	Rotation.Yaw += DeltaTime * 45.f;
	SetActorRotation(Rotation);
}

void ACoin::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr)
		return;


	AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
	if (Character)
	{
		Character->TakeCoin(CoinAmount);
		Destroy();
	}
}
