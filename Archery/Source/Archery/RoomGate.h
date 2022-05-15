// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomGate.generated.h"

UCLASS()
class ARCHERY_API ARoomGate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomGate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate", Meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* GateMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate", Meta = (AllowPrivateAccess = "true"))
	class USphereComponent* WidgetSphere = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate", Meta = (AllowPrivateAccess = "true"))
	int32 MaxCoinAmount = 10000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate", Meta = (AllowPrivateAccess = "true"))
	int32 CurrentCoinAmount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate", Meta = (AllowPrivateAccess = "true"))
	bool IsCharacterOn = false;

	bool IsGateOn = false;

	FTimerHandle GateTimerHandle;
	float GateDisappearTime = 5.f;
public:
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void HideWidget();

	void PutCurrentCoin(int32 CoinAmountToPut);

	void OpenGate();

	void DestroyThis();

	int32 GetCurrentCoinAmount();
	int32 GetMaxCoinAmount();
};
