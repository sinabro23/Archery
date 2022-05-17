// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERY_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainPlayerController();


protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	class AMainCharacter* MainCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> SkillHUDOverlayAsset;

	UPROPERTY()
	class UUserWidget* SkillHUDOverlay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	UPROPERTY()
	class UUserWidget* HUDOverlay; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> TapHUDOverlayAsset;

	UPROPERTY()
	class UUserWidget* TapHUDOverlay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> EndingHUDOverlayAsset;

	UPROPERTY()
	class UUserWidget* EndingHUDOverlay;

private:

	bool IsTapOn = false;

	class USoundCue* BGMSound;

public:
	
	void SetWidgetVisiblity(bool Visible);

	void SetTapHUDVisibility(bool Visible);

	void SetEndingHUDVisibility(bool Visible);
};
