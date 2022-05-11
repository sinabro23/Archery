// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "HPPotion.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERY_API AHPPotion : public AItem
{
	GENERATED_BODY()
	
public:
	AHPPotion();

protected:
	virtual void UseItem(class AMainCharacter* CharacterToUse) override;
	virtual void PostInitializeComponents() override;

private:
	float HealAmount = 30.f;

public:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
