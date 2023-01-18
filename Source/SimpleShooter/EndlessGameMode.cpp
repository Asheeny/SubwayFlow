// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessGameMode.h"
#include "GameFramework/Controller.h"
#include "EngineUtils.h"

void AEndlessGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());

	if (PlayerController != nullptr)
	{
		PlayerController->GameHasEnded(PlayerController->GetPawn(), false);
	}
}
