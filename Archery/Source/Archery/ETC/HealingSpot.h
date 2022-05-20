// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealingSpot.generated.h"

UCLASS()
class ARCHERY_API AHealingSpot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealingSpot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealingSpot", Meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* HealingSpotParticle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealingSpot", Meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* HealingEffectPaticle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealingSpot", Meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereComponent = nullptr;

public:
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
