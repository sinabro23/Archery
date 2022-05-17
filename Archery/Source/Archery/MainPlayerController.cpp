// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "MainCharacter.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

AMainPlayerController::AMainPlayerController()
{
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_BGM(TEXT("SoundCue'/Game/_Game/Assets/Sounds/BGM/battle-of-the-dragons-8037_Cue.battle-of-the-dragons-8037_Cue'"));
	if (SC_BGM.Succeeded())
	{
		BGMSound = SC_BGM.Object;
	}
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (SkillHUDOverlayAsset)
	{
		SkillHUDOverlay = CreateWidget<UUserWidget>(this, SkillHUDOverlayAsset);
	}

	if (SkillHUDOverlay)
	{
		SkillHUDOverlay->AddToViewport();
		SkillHUDOverlay->SetVisibility(ESlateVisibility::Hidden);
	}

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}

	if (HUDOverlay)
	{
		HUDOverlay->AddToViewport();
		HUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}

	if (TapHUDOverlayAsset)
	{
		TapHUDOverlay = CreateWidget<UUserWidget>(this, TapHUDOverlayAsset);
	}

	if (TapHUDOverlay)
	{
		TapHUDOverlay->AddToViewport();
		TapHUDOverlay->SetVisibility(ESlateVisibility::Hidden);
	}

	if (EndingHUDOverlayAsset)
	{
		EndingHUDOverlay = CreateWidget<UUserWidget>(this, EndingHUDOverlayAsset);
	}

	if (TapHUDOverlay)
	{
		EndingHUDOverlay->AddToViewport();
		EndingHUDOverlay->SetVisibility(ESlateVisibility::Hidden);
	}

	MainCharacter = Cast<AMainCharacter>(GetOwner());

	if (BGMSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), BGMSound);
	}
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AMainPlayerController::SetWidgetVisiblity(bool Visible)
{
	if (Visible == true)
	{
		SkillHUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SkillHUDOverlay->SetVisibility(ESlateVisibility::Hidden);
	}

	
}

void AMainPlayerController::SetTapHUDVisibility(bool Visible)
{
	if (Visible == true)
	{
		TapHUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		TapHUDOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::SetEndingHUDVisibility(bool Visible)
{
	if (Visible == true)
	{
		EndingHUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		EndingHUDOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}


