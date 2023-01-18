// Fill out your copyright notice in the Description page of Project Settings.


#include "ThePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AThePlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (bIsWinner)
	{
		if (WinScreenClass != nullptr)
		{
			UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
			WinScreen->AddToViewport();
		}
	}
	else
	{
		if (LoseScreenClass != nullptr)
		{
			UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
			LoseScreen->AddToViewport();
		}
	}

	HUD->RemoveFromViewport();
	//GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}

void AThePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDClass != nullptr)
	{
		HUD = CreateWidget(this, HUDClass);
		HUD->AddToViewport();
	}
}