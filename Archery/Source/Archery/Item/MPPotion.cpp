// Fill out your copyright notice in the Description page of Project Settings.


#include "MPPotion.h"
#include "MainCharacter.h"	
#include "Components/SphereComponent.h"

AMPPotion::AMPPotion()
{
	ItemName = FName("MPPotion");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MPPotion(TEXT("StaticMesh'/Game/_Game/Assets/Potion/Potion_3.Potion_3'"));
	if (SM_MPPotion.Succeeded())
	{
		ItemMesh->SetStaticMesh(SM_MPPotion.Object);
	}
}

void AMPPotion::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AMPPotion::OnBeginOverlap);
}

void AMPPotion::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr)
		return;


	AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
	if (Character)
	{
		Character->TakeMPPotion();
		Destroy();
	}
}
