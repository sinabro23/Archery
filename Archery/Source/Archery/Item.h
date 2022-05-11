// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class ARCHERY_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void UseItem(class AMainCharacter* CharacterToUse);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemName = FName("None");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UStaticMeshComponent* ItemMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class USphereComponent* SphereComp = nullptr;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

};
