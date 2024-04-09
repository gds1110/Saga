// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnEnd.h"
#include "Character/CharacterBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnEnd::UBTTask_TurnEnd()
{
	NodeName = TEXT("TurnEnd");

}

EBTNodeResult::Type UBTTask_TurnEnd::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	auto ControllingPawn = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;

	UE_LOG(LogTemp, Log, TEXT("Turn End"));
	ControllingPawn->TurnEnd();

	return EBTNodeResult::Succeeded;
}
