// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Coin.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERY_API ACoin : public AItem
{
	GENERATED_BODY()
	
public:
	ACoin();

protected:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
private:
	int32 CoinAmount = 1500;

public:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FORCEINLINE void SetCoinAmount(int32 NewCoin) { CoinAmount = NewCoin; }
};
