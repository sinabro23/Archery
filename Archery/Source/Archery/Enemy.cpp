// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnAttacked(float DamageAmount)
{
	float NewHP = CurrentHP - DamageAmount;
	SetHP(NewHP);
}

float AEnemy::GetMaxHP()
{
	return MaxHP;
}

float AEnemy::GetCurrentHP()
{
	return CurrentHP;
}

float AEnemy::GetHPRatio()
{
	return CurrentHP / MaxHP;
}

void AEnemy::SetHP(float NewHP)
{
	CurrentHP = NewHP;

	if (CurrentHP < 0.f)
		CurrentHP = 0.f;

	OnHPChanged.Broadcast();
}

