// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_IsPlayerSeen.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"

UBTService_IsPlayerSeen::UBTService_IsPlayerSeen()
{
	NodeName = TEXT("Update PlayerLoc If Seen");
}

void UBTService_IsPlayerSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (PlayerPawn == nullptr || Controller == nullptr)
		return;

	if (Controller->LineOfSightTo(PlayerPawn))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
	}

	else
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
}