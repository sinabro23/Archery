// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "MPPotion.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERY_API AMPPotion : public AItem
{
	GENERATED_BODY()

public:
	AMPPotion();

protected:
	virtual void PostInitializeComponents() override;

private:
	float HealAmount = 30.f;

public:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
