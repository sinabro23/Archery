// Fill out your copyright notice in the Description page of Project Settings.


#include "HPPotion.h"
#include "MainCharacter.h"	

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
