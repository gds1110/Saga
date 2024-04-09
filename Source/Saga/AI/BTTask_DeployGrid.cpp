// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_DeployGrid.h"
#include "AI/SAAI.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CharacterBase.h"
#include "Grid/Grid.h"



EBTNodeResult::Type UBTTask_DeployGrid::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;


	ControllingPawn->SpellGridDeploy();



	return EBTNodeResult::Succeeded;
}

UBTTask_DeployGrid::UBTTask_DeployGrid()
{
	NodeName = TEXT("DeployGrids");

}
