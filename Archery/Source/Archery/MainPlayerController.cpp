// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

AMainPlayerController::AMainPlayerController()
{

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
