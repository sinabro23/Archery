// Fill out your copyright notice in the Description page of Project Settings.


#include "HPPotion.h"
#include "MainCharacter.h"	
#include "Components/SphereComponent.h"

AHPPotion::AHPPotion()
{
	ItemName = FName("HPPotion");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_HPPotion(TEXT("StaticMesh'/Game/AlchemistsHouse/Assets/Models/Potion_1.Potion_1'"));
	if (SM_HPPotion.Succeeded())
	{
		ItemMesh->SetStaticMesh(SM_HPPotion.Object);
	}
}

void AHPPotion::UseItem(AMainCharacter* CharacterToUse)
{
	float HP = CharacterToUse->GetCurrentHP();
	CharacterToUse->SetHP(HP + HealAmount);
}

void AHPPotion::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AHPPotion::OnBeginOverlap);
}

void AHPPotion::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr)
		return;


	AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
	if (Character)
	{
		Character->TakeHPPotion();
		Destroy();
	}

}
