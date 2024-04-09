// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_MoveToGrid.h"
#include "AI/SAAI.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CharacterBase.h"
#include "Grid/Grid.h"
#include "Grid/GridManager.h"
#include "AStar/AstarActorComponent.h"
#include "AI/SpawnAIController.h"


UBTTask_MoveToGrid::UBTTask_MoveToGrid()
{
	NodeName = TEXT("MoveGridTask");
	bNotifyTick = true; 
	bIsMoving = false;
}

EBTNodeResult::Type UBTTask_MoveToGrid::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed; // 예외처리. 	
	AGridManager* GridManager = ControllingPawn->GetGridManager();
	UE_LOG(LogTemp, Log, TEXT("Enter Move To Grid"));

	if (GridManager == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	AGrid* DestGrid = Cast<AGrid>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_DESTGRID));
	if (DestGrid == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	if (ControllingPawn->AIControl->CommandDataArray.Num() > 0)
	{
		
	}
	else
	{
		GridManager->ClearPathStateGrids();
		TArray<AGrid*> PathToDest;
		PathToDest = GridManager->GetAstartCompo()->GetPathFInd(ControllingPawn->GetCharacterGrid(), DestGrid);
		GridManager->RefreshGrids();
		ControllingPawn->AddCommands(CommandMove, PathToDest);
		ControllingPawn->DoCommand();
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_MoveToGrid::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	auto ControllingPawn = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	AGrid* DestGrid = Cast<AGrid>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_DESTGRID));
	if (ControllingPawn->bIsMoving == false||ControllingPawn->GetCharacterGrid()== DestGrid)
	{
		UE_LOG(LogTemp, Log, TEXT("Finish Move"));

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);//태스크 종료함수. 테스크 성공 반환. 
	}
}
