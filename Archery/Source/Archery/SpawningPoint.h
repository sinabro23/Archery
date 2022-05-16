// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawningPoint.generated.h"

UCLASS()
class ARCHERY_API ASpawningPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawningPoint();

	static int32 SpawnedCount;

	static int32 SpawnPointCounts;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	class USphereComponent* CheckSphere = nullptr;
	int32 EnemyCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", Meta = (AllowPrivateAccess = "true"))
	FVector RandVector = FVector();
	FVector BeforeRandVector = FVector();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	FVector SpawnPoint1 = FVector();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	FVector SpawnPoint2 = FVector();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	FVector SpawnPoint3 = FVector();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	FVector SpawnPoint4 = FVector();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", Meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	FVector SpawnPoint5 = FVector();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", Meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	FVector SpawnPoint6 = FVector();

	static bool bIsBeginSpawn;
	bool bIsAlreadySpawn = false;

	UPROPERTY(VisibleAnywhere)
	bool IsButtonPressed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", Meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	UStaticMeshComponent* SpawnButtonMesh = nullptr;

	float DefaultZHeight = 0.f;

	FTimerHandle ButtonTimer;
	float ButtonPresssingTime = 0.5f;
public:
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void CheckIsThereEnemy();

	UFUNCTION(BlueprintImplementableEvent)
	void EnemySpawn();

	void SetRandVector();

	UFUNCTION(BlueprintCallable)
	void RaiseSpawnCount();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void HideWidget();

	void PressButton();

	void ResetButton();
};
