// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

AMainPlayerController::AMainPlayerController()
{

}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}

	if (HUDOverlay)
	{
		HUDOverlay->AddToViewport();
		HUDOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::SetWidgetVisiblity(bool Visible)
{
	if (Visible == true)
	{
		HUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		HUDOverlay->SetVisibility(ESlateVisibility::Hidden);
	}

}
