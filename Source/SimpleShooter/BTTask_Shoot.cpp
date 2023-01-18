// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "AIController.h"
#include "ShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr)
		return EBTNodeResult::Failed;
	if(UGameplayStatics::GetPlayerPawn(GetWorld(), 0) == nullptr)
		return EBTNodeResult::Failed;
	
	AShooterCharacter* Character = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if(Character == nullptr)
		return EBTNodeResult::Failed;
	
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("FireRate"), FMath::RandRange(0.6f, 1.2f));
	Character->UsePrimaryFire();

	return EBTNodeResult::Succeeded;
}