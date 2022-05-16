// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawningPoint.h"
#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "Enemy.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainCharacter.h"

int32 ASpawningPoint::SpawnedCount = 0;
int32 ASpawningPoint::SpawnPointCounts = 0;
bool ASpawningPoint::bIsBeginSpawn = false;

// Sets default values
ASpawningPoint::ASpawningPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	SetRootComponent(SpawnButtonMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SpawnButton(TEXT("StaticMesh'/Game/ParagonProps/Monolith/Ruins/Meshes/Ruins_Player_Pedestal_02.Ruins_Player_Pedestal_02'"));
	if (SM_SpawnButton.Succeeded())
	{
		SpawnButtonMesh->SetStaticMesh(SM_SpawnButton.Object);
	}

	CheckSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CheckSphere"));
	CheckSphere->SetupAttachment(GetRootComponent());
	CheckSphere->SetSphereRadius(500.f);


}

// Called when the game starts or when spawned
void ASpawningPoint::BeginPlay()
{
	Super::BeginPlay();
	SpawnPointCounts++;
	BeforeRandVector = GetActorLocation() + FVector(0.0f, 0.0f, 100.f);
	RandVector = GetActorLocation() + FVector(0.0f, 0.0f, 100.f);
	DefaultZHeight = GetActorLocation().Z;

	SpawnPoint1 = UKismetMathLibrary::TransformLocation(GetActorTransform(), SpawnPoint1);
	SpawnPoint2 = UKismetMathLibrary::TransformLocation(GetActorTransform(), SpawnPoint2);
	SpawnPoint3 = UKismetMathLibrary::TransformLocation(GetActorTransform(), SpawnPoint3);
	SpawnPoint4 = UKismetMathLibrary::TransformLocation(GetActorTransform(), SpawnPoint4);
	SpawnPoint5 = UKismetMathLibrary::TransformLocation(GetActorTransform(), SpawnPoint5);
	SpawnPoint6 = UKismetMathLibrary::TransformLocation(GetActorTransform(), SpawnPoint6);

	EnemySpawn();
}

void ASpawningPoint::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CheckSphere->OnComponentBeginOverlap.AddDynamic(this, &ASpawningPoint::OnSphereBeginOverlap);
	CheckSphere->OnComponentEndOverlap.AddDynamic(this, &ASpawningPoint::OnSphereEndOverlap);


}

// Called every frame
void ASpawningPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsButtonPressed)
	{
		FVector NewButtonVector = GetActorLocation();
		NewButtonVector.Z = GetActorLocation().Z - DeltaTime * 80.f;
		SetActorLocation(NewButtonVector);
		
	}
	else if(GetActorLocation().Z <= DefaultZHeight)
	{
		FVector NewButtonVector = GetActorLocation();
		NewButtonVector.Z = GetActorLocation().Z + DeltaTime * 80.f;
		SetActorLocation(NewButtonVector);
	}
}

void ASpawningPoint::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
		return;

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter)
	{
		MainCharacter->SetIsOnSpawnButton(true, this);
		ShowWidget();
	}
}

void ASpawningPoint::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor)
		return;

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter)
	{
		MainCharacter->SetIsOnSpawnButton(false, nullptr);
		HideWidget();
	}
}

void ASpawningPoint::CheckIsThereEnemy()
{
	FVector Center = GetActorLocation();
	TArray<FOverlapResult> HitResults;
	FCollisionQueryParams CollsionQueryParam(NAME_None, false, this);

	bool bResult = GetWorld()->OverlapMultiByChannel(
		HitResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(400.f),
		CollsionQueryParam
	);

	int32 Count = 0;

	if (bResult)
	{
		for (auto HitResult : HitResults)
		{
			if (HitResult.Actor.IsValid())
			{
				AEnemy* HitEnemy = Cast<AEnemy>(HitResult.Actor);

				if (HitEnemy)
				{
					Count++;
				}
			}
		}
	}

	EnemyCount = Count;

	DrawDebugSphere(GetWorld(), Center, 400.f, 16, FColor::Green, false, 0.5f);
}

void ASpawningPoint::SetRandVector()
{
	FVector RandPoint = BeforeRandVector + FVector(FMath::RandRange(-100.f, 100.f), FMath::RandRange(-100.f, 100.f), 0.f);
	RandVector = RandPoint;
}

void ASpawningPoint::RaiseSpawnCount()
{
	SpawnedCount++;
}

void ASpawningPoint::PressButton()
{
	IsButtonPressed = true;
	GetWorldTimerManager().SetTimer(ButtonTimer, this, &ASpawningPoint::ResetButton, ButtonPresssingTime);
	EnemySpawn();
}

void ASpawningPoint::ResetButton()
{
	IsButtonPressed = false;
}




