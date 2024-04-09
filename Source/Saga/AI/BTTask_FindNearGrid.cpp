// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindNearGrid.h"
#include "AI/SAAI.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CharacterBase.h"
#include "Grid/Grid.h"
#include "Kismet/GameplayStatics.h"


UBTTask_FindNearGrid::UBTTask_FindNearGrid()
{
	NodeName = TEXT("FindDest");
}

EBTNodeResult::Type UBTTask_FindNearGrid::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	auto ControllingPawn = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed; // 예외처리. 	
	auto Target = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == Target) return EBTNodeResult::Failed; // 예외처리. 	

	 //(myActor1->GetActorLocation() - myActor2->GetActorLocation()).Size();

	TArray<AGrid*> MoveGrids = ControllingPawn->GetCharacterArea();
	if (MoveGrids.Num() <= 0)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_DESTGRID, nullptr);

		return EBTNodeResult::Failed;
	}
	AGrid* DestGrid = MoveGrids[0];

	// 경로에 공격가능한 자리 있는지 확인
	bool CanMoveAtk = false;
	bool CanAtk = false;
	float Longist = -1;
	float TarDistance = 0;
	AGrid* TargetGrid = Target->GetCharacterGrid();
	float ClosedGridDistance = TNumericLimits<float>::Max();
	float GridDistance = 0;
	
	for (int i = 0; i < MoveGrids.Num(); i++)
	{
		CanAtk = ControllingPawn->CalculateAttack(MoveGrids[i], TargetGrid);
		TarDistance = (Target->GetActorLocation() - MoveGrids[i]->GetActorLocation()).Size(); //상대방과 예비그리드의 거리
		GridDistance = (ControllingPawn->GetActorLocation() - MoveGrids[i]->GetActorLocation()).Size(); //내 그리드와 예비그리드의 거리
		if (CanAtk == true)
		{
			CanMoveAtk = true;
			if (TarDistance > Longist && GridDistance < ClosedGridDistance)
			{
				Longist = TarDistance;
				ClosedGridDistance = GridDistance;
				DestGrid = MoveGrids[i];
				UE_LOG(LogTemp, Log, TEXT("Target : %s , Distance : %f"), *MoveGrids[i]->GetName(), Longist);
			}
		}
	}
	if (CanMoveAtk == false)
	{
		float ClosedDistance = TNumericLimits<float>::Max();
		float Distance;

		for (int i = 0; i < MoveGrids.Num(); i++)
		{
			Distance = (Target->GetActorLocation() - MoveGrids[i]->GetActorLocation()).Size();
			//UE_LOG(LogTemp, Log, TEXT("Target : %s , Distance : %f"),*MoveGrids[i]->GetName(),Distance);

			if (Distance < ClosedDistance)
			{
				ClosedDistance = Distance;
				DestGrid = MoveGrids[i];
			}
		}
	}
	if (DestGrid != nullptr)
	{
		ControllingPawn->FocusTarget(DestGrid);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_DESTGRID, DestGrid);
		return EBTNodeResult::Succeeded;

	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_DESTGRID, nullptr);
		return EBTNodeResult::Failed;

	}

	return EBTNodeResult::Succeeded;



}
