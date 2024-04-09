// Fill out your copyright notice in the Description page of Project Settings.


#include "AStar/AstarActorComponent.h"
#include "Algo/Reverse.h"
#include "Grid/Grid.h"
#include "Grid/GridManager.h"
#include "Character/CharacterBase.h"


// Sets default values for this component's properties
UAstarActorComponent::UAstarActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	curCharacter = CreateDefaultSubobject<AActor>(TEXT("CurCharacter"));
}


// Called when the game starts
void UAstarActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	curCharacter = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UAstarActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//UE_LOG(LogTemp, Log, TEXT("X : %f , Y : %f"), curCharacter->GetActorLocation().X, curCharacter->GetActorLocation().Y);

}
TArray<class AGrid*> UAstarActorComponent::GetPathArea(ACharacterBase* InputCharacter)
{
	//캐릭터 이동범위 찾는함수

	return GetPathArea(InputCharacter->GetCharacterGrid());
}

TArray<class AGrid*> UAstarActorComponent::GetPathArea(AGrid* StartGrid)
{
	//캐릭터 이동범위 찾는함수

	GridX = GridManager->GetGridX();
	GridY = GridManager->GetGridY();
	OpenList = TArray<TObjectPtr<AGrid>>();
	ClosedList = TArray<TObjectPtr<AGrid>>();
	FinalNodeList = TArray<TObjectPtr<AGrid>>();

	int32 Stamina = StartGrid->OnCharacter->GetCharacterStamina();

	OpenList.Add(StartGrid);

	while (OpenList.Num() > 0)
	{
		// 열린리스트 중 가장 F가 작고 F가 같다면 H가 작은 걸 현재노드로 하고 열린리스트에서 닫힌 리스트로 옮기기
		CurNode = OpenList[0];
		for (int i = 0; i < OpenList.Num(); i++)
		{
			if (OpenList[i]->Cost <= CurNode->Cost)
			{
				CurNode = OpenList[i];
			}

		}

		OpenList.Remove(CurNode);
		ClosedList.Add(CurNode);
		if (!OpenCostList.Contains(CurNode))
		{
			OpenCostList.Add(CurNode);
		}

		//// 우상,좌상,좌하,우하
		//OpneCostListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y + 1, Stamina);
		//OpneCostListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y + 1, Stamina);
		//OpneCostListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y - 1, Stamina);
		//OpneCostListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y - 1, Stamina);

		//상 우 하 좌
		OpneCostListAdd(CurNode->GridIndex.X, CurNode->GridIndex.Y + 1, Stamina);
		OpneCostListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y, Stamina);
		OpneCostListAdd(CurNode->GridIndex.X, CurNode->GridIndex.Y - 1, Stamina);
		OpneCostListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y, Stamina);
	}

	TArray<AGrid*> RetGrid = OpenCostList;
	
	for (AGrid* TempGrid : OpenCostList)
	{
		TempGrid->Cost = 0;
	}
	for (AGrid* TempGrid : ClosedList)
	{
		TempGrid->Cost = 0;
	}

	OpenCostList.Empty();
	ClosedList.Empty();
	return RetGrid;
}

TArray<class AGrid*> UAstarActorComponent::GetPathFInd(class AGrid* startNode, class AGrid* targetNode)
{
	
	if (targetNode == nullptr)
	{
		FinalNodeList = TArray<TObjectPtr<AGrid>>();
		return FinalNodeList;
	}
	if (startNode == nullptr)
	{
		FinalNodeList = TArray<TObjectPtr<AGrid>>();
		return FinalNodeList;
	}
	if (startNode->GetOnCharacter() != nullptr)
	{
		if (!startNode->GetOnCharacter()->GetCharacterArea().Contains(targetNode))
		{
			FinalNodeList = TArray<TObjectPtr<AGrid>>();
			return FinalNodeList;
		}
	}


	//Finalnodelist 내용물 초기화.
	StartNode = startNode;
	TargetNode = targetNode;
	GridX = GridManager->GetGridX();
	GridY = GridManager->GetGridY();
	OpenList = TArray<TObjectPtr<AGrid>>();
	ClosedList = TArray<TObjectPtr<AGrid>>();
	FinalNodeList = TArray<TObjectPtr<AGrid>>();

	OpenList.Add(StartNode);

	while (OpenList.Num() > 0)
	{
		// 열린리스트 중 가장 F가 작고 F가 같다면 H가 작은 걸 현재노드로 하고 열린리스트에서 닫힌 리스트로 옮기기
		CurNode = OpenList[0];
		for (int i = 0; i < OpenList.Num(); i++)
		{
			/*if (OpenList[i]->F <= CurNode->F && OpenList[i]->Heuristic < CurNode->Heuristic)
			{
				CurNode = OpenList[i];
			}*/
			if (OpenList[i]->F < CurNode->F || OpenList[i]->F == CurNode->F && OpenList[i]->Heuristic < CurNode->Heuristic)
			{
				CurNode = OpenList[i];
			}
		}
		OpenList.Remove(CurNode);
		ClosedList.Add(CurNode);
		//CurNode 초기화
		if (CurNode == TargetNode)
		{
			TObjectPtr<AGrid> TargetCurNode = TargetNode;
			while (TargetCurNode != StartNode)
			{
				FinalNodeList.Add(TargetCurNode);
				TargetCurNode = TargetCurNode->ParentNode;
			}
			FinalNodeList.Add(StartNode);
			Algo::Reverse(FinalNodeList);
			for (int i = 0; i < FinalNodeList.Num(); i++)
			{
				//UE_LOG(LogTemp, Log, TEXT("%d x: %d, y: %d"), i, FinalNodeList[i]->GridIndex.X, FinalNodeList[i]->GridIndex.Y);
				if (i == 0)
				{
					//FinalNodeList[i]->PathState = EPathState::PATH;
					continue;
				}
				if (i == FinalNodeList.Num() - 1)
				{
					FinalNodeList[i]->PathState = EPathState::TARGET;
					break;
				}
				FinalNodeList[i]->PathState = EPathState::PATH;
			}
			if (FinalNodeList.Num() > 0)
			{
				FinalNodeList.RemoveAt(0);
			}
			return FinalNodeList;
		}
		bool allowDiagonal = false;
		// 대각선 허용시
		if (allowDiagonal)
		{
			// 우상,좌상,좌하,우하
			OpneListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y + 1);
			OpneListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y + 1);
			OpneListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y - 1);
			OpneListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y - 1);
		}
		//상 우 하 좌
		OpneListAdd(CurNode->GridIndex.X, CurNode->GridIndex.Y + 1);
		OpneListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y);
		OpneListAdd(CurNode->GridIndex.X, CurNode->GridIndex.Y - 1);
		OpneListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y);
	}

	return FinalNodeList;
}

void UAstarActorComponent::PathFinding()
{

	StartNode = GridArray[0][0];
	TargetNode = GridArray[6][6];
	UE_LOG(LogTemp, Log, TEXT("StartNode x: %d, y: %d"), StartNode->GetGridIndex().X, StartNode->GetGridIndex().Y);
	UE_LOG(LogTemp, Log, TEXT("TargetNode x: %d, y: %d"), TargetNode->GetGridIndex().X, TargetNode->GetGridIndex().Y);
	GridX = GridManager->GetGridX();
	GridY = GridManager->GetGridY();
	OpenList = TArray<TObjectPtr<AGrid>>();
	ClosedList = TArray<TObjectPtr<AGrid>>();
	FinalNodeList = TArray<TObjectPtr<AGrid>>();

	OpenList.Add(StartNode);

	while (OpenList.Num() > 0)
	{
		// 열린리스트 중 가장 F가 작고 F가 같다면 H가 작은 걸 현재노드로 하고 열린리스트에서 닫힌 리스트로 옮기기
		CurNode = OpenList[0];
		for (int i = 0; i < OpenList.Num(); i++)
		{
			/*if (OpenList[i]->F <= CurNode->F && OpenList[i]->Heuristic < CurNode->Heuristic)
			{
				CurNode = OpenList[i];
			}*/
			if (OpenList[i]->F < CurNode->F || OpenList[i]->F == CurNode->F && OpenList[i]->Heuristic < CurNode->Heuristic)
			{
				CurNode = OpenList[i];
			}
		}
		OpenList.Remove(CurNode);
		ClosedList.Add(CurNode);

		if (CurNode == TargetNode)
		{
			TObjectPtr<AGrid> TargetCurNode = TargetNode;
			while (TargetCurNode != StartNode)
			{
				FinalNodeList.Add(TargetCurNode);
				TargetCurNode = TargetCurNode->ParentNode;
			}
			FinalNodeList.Add(StartNode);
			Algo::Reverse(FinalNodeList);

			for (int i = 0; i < FinalNodeList.Num(); i++)
			{
				UE_LOG(LogTemp, Log, TEXT("grid : %d x: %d, y: %d"), i, FinalNodeList[i]->GridIndex.X, FinalNodeList[i]->GridIndex.Y);
				if (i == 0)
				{
					FinalNodeList[i]->ChangeGridPathState(EPathState::START);
					continue;
				}
				if (i == FinalNodeList.Num() - 1)
				{
					FinalNodeList[i]->ChangeGridPathState(EPathState::TARGET);
					break;
				}
				FinalNodeList[i]->ChangeGridPathState(EPathState::PATH);
			}
			return;
		}
		bool allowDiagonal = false;
		// 대각선 허용시
		if (allowDiagonal)
		{
			// 우상,좌상,좌하,우하
			OpneListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y + 1);
			OpneListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y + 1);
			OpneListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y - 1);
			OpneListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y - 1);
		}
		//상 우 하 좌
		OpneListAdd(CurNode->GridIndex.X, CurNode->GridIndex.Y + 1);
		OpneListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y);
		OpneListAdd(CurNode->GridIndex.X, CurNode->GridIndex.Y - 1);
		OpneListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y);
	}
}

void UAstarActorComponent::PathFinding(AGrid* startNode, AGrid* targetNode)
{
	if (targetNode == nullptr)
	{
		return;
	}
	//Finalnodelist 내용물 초기화.
	StartNode = startNode;
	TargetNode = targetNode;
	GridX = GridManager->GetGridX();
	GridY = GridManager->GetGridY();
	OpenList = TArray<TObjectPtr<AGrid>>();
	ClosedList = TArray<TObjectPtr<AGrid>>();
	FinalNodeList = TArray<TObjectPtr<AGrid>>();

	OpenList.Add(StartNode);

	while (OpenList.Num() > 0)
	{
		// 열린리스트 중 가장 F가 작고 F가 같다면 H가 작은 걸 현재노드로 하고 열린리스트에서 닫힌 리스트로 옮기기
		CurNode = OpenList[0];
		for (int i = 0; i < OpenList.Num(); i++)
		{
			/*if (OpenList[i]->F <= CurNode->F && OpenList[i]->Heuristic < CurNode->Heuristic)
			{
				CurNode = OpenList[i];
			}*/
			if (OpenList[i]->F < CurNode->F || OpenList[i]->F == CurNode->F && OpenList[i]->Heuristic < CurNode->Heuristic)
			{
				CurNode = OpenList[i];
			}
		}
		OpenList.Remove(CurNode);
		ClosedList.Add(CurNode);
		//CurNode 초기화
		if (CurNode == TargetNode)
		{
			TObjectPtr<AGrid> TargetCurNode = TargetNode;
			while (TargetCurNode != StartNode)
			{
				FinalNodeList.Add(TargetCurNode);
				TargetCurNode = TargetCurNode->ParentNode;
			}
			FinalNodeList.Add(StartNode);
			Algo::Reverse(FinalNodeList);

			for (int i = 0; i < FinalNodeList.Num(); i++)
			{
				UE_LOG(LogTemp, Log, TEXT("%d x: %d, y: %d"), i, FinalNodeList[i]->GridIndex.X, FinalNodeList[i]->GridIndex.Y);
				if (i == 0)
				{
					FinalNodeList[i]->ChangeGridPathState(EPathState::START);
					continue;
				}
				if (i == FinalNodeList.Num() - 1)
				{
					FinalNodeList[i]->ChangeGridPathState(EPathState::TARGET);
					break;
				}
				FinalNodeList[i]->ChangeGridPathState(EPathState::PATH);
			}
			return;
		}
		bool allowDiagonal = false;
		// 대각선 허용시
		if (allowDiagonal)
		{
			// 우상,좌상,좌하,우하
			OpneListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y + 1);
			OpneListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y + 1);
			OpneListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y - 1);
			OpneListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y - 1);
		}
		//상 우 하 좌
		OpneListAdd(CurNode->GridIndex.X, CurNode->GridIndex.Y + 1);
		OpneListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y);
		OpneListAdd(CurNode->GridIndex.X, CurNode->GridIndex.Y - 1);
		OpneListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y);
	}

}

int32 UAstarActorComponent::GetPathFindingCost(AGrid* startNode, AGrid* targetNode)
{
	if (targetNode == nullptr)
	{
		return 0;
	}
	//Finalnodelist 내용물 초기화.
	StartNode = startNode;
	TargetNode = targetNode;
	GridX = GridManager->GetGridX();
	GridY = GridManager->GetGridY();
	OpenList = TArray<TObjectPtr<AGrid>>();
	ClosedList = TArray<TObjectPtr<AGrid>>();
	FinalNodeList = TArray<TObjectPtr<AGrid>>();
	int32 RetCostValue = 0;
	OpenList.Add(StartNode);

	while (OpenList.Num() > 0)
	{
		// 열린리스트 중 가장 F가 작고 F가 같다면 H가 작은 걸 현재노드로 하고 열린리스트에서 닫힌 리스트로 옮기기
		CurNode = OpenList[0];
		for (int i = 0; i < OpenList.Num(); i++)
		{
			/*if (OpenList[i]->F <= CurNode->F && OpenList[i]->Heuristic < CurNode->Heuristic)
			{
				CurNode = OpenList[i];
			}*/
			if (OpenList[i]->F < CurNode->F || OpenList[i]->F == CurNode->F && OpenList[i]->Heuristic < CurNode->Heuristic)
			{
				CurNode = OpenList[i];
			}
		}
		OpenList.Remove(CurNode);
		ClosedList.Add(CurNode);
		//CurNode 초기화
		if (CurNode == TargetNode)
		{
			TObjectPtr<AGrid> TargetCurNode = TargetNode;
			while (TargetCurNode != StartNode)
			{
				FinalNodeList.Add(TargetCurNode);
				TargetCurNode = TargetCurNode->ParentNode;
			}
			FinalNodeList.Add(StartNode);
			Algo::Reverse(FinalNodeList);

			for (int i = 0; i < FinalNodeList.Num(); i++)
			{
				UE_LOG(LogTemp, Log, TEXT("%d x: %d, y: %d"), i, FinalNodeList[i]->GridIndex.X, FinalNodeList[i]->GridIndex.Y);
				if (i == 0)
				{
					FinalNodeList[i]->ChangeGridPathState(EPathState::START);
					continue;
				}
				if (i == FinalNodeList.Num() - 1)
				{
					FinalNodeList[i]->ChangeGridPathState(EPathState::TARGET);
					break;
				}
				FinalNodeList[i]->ChangeGridPathState(EPathState::PATH);
			}
			 RetCostValue = (FinalNodeList.Num() * 10);
			return RetCostValue;
		}
		bool allowDiagonal = false;
		// 대각선 허용시
		if (allowDiagonal)
		{
			// 우상,좌상,좌하,우하
			OpneListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y + 1);
			OpneListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y + 1);
			OpneListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y - 1);
			OpneListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y - 1);
		}
		//상 우 하 좌
		OpneListAdd(CurNode->GridIndex.X, CurNode->GridIndex.Y + 1);
		OpneListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y);
		OpneListAdd(CurNode->GridIndex.X, CurNode->GridIndex.Y - 1);
		OpneListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y);
	}
	RetCostValue = (FinalNodeList.Num() * 10);
	return RetCostValue;
}

void UAstarActorComponent::PathFindingNearCost()
{
	//if target is contais finalcostlist
	//Stamina - target cost

}

void UAstarActorComponent::PathFindingNearCost(int32 Stamina)
{
	// if select Character

	GridX = GridManager->GetGridX();
	GridY = GridManager->GetGridY();
	OpenList = TArray<TObjectPtr<AGrid>>();
	ClosedList = TArray<TObjectPtr<AGrid>>();
	FinalNodeList = TArray<TObjectPtr<AGrid>>();

	
	AGrid* startGrid = GridArray[15][15];
	OpenList.Add(startGrid);

	while (OpenList.Num() > 0)
	{
		// 열린리스트 중 가장 F가 작고 F가 같다면 H가 작은 걸 현재노드로 하고 열린리스트에서 닫힌 리스트로 옮기기
		CurNode = OpenList[0];
		for (int i = 0; i < OpenList.Num(); i++)
		{
			if (OpenList[i]->Cost <= CurNode->Cost)
			{
				CurNode = OpenList[i];
			}
		
		}

		OpenList.Remove(CurNode);
		ClosedList.Add(CurNode);
		if (!OpenCostList.Contains(CurNode))
		{
			OpenCostList.Add(CurNode);
		}

		// 우상,좌상,좌하,우하
		OpneCostListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y + 1,Stamina);
		OpneCostListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y + 1,Stamina);
		OpneCostListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y - 1,Stamina);
		OpneCostListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y - 1,Stamina);
		
		//상 우 하 좌
		OpneCostListAdd(CurNode->GridIndex.X, CurNode->GridIndex.Y + 1,Stamina);
		OpneCostListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y,Stamina);
		OpneCostListAdd(CurNode->GridIndex.X, CurNode->GridIndex.Y - 1,Stamina);
		OpneCostListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y,Stamina);
	}
	for (int32 i = 0; i < OpenCostList.Num(); i++)
	{
		OpenCostList[i]->ChangeGridPathState(EPathState::PATH);
	}
}

void UAstarActorComponent::PathFindingNearCost(AGrid* startNode)
{

	GridX = GridManager->GetGridX();
	GridY = GridManager->GetGridY();
	OpenList = TArray<TObjectPtr<AGrid>>();
	ClosedList = TArray<TObjectPtr<AGrid>>();
	FinalNodeList = TArray<TObjectPtr<AGrid>>();

	int32 Stamina = startNode->OnCharacter->GetCharacterStamina();
	
	OpenList.Add(startNode);

	while (OpenList.Num() > 0)
	{
		// 열린리스트 중 가장 F가 작고 F가 같다면 H가 작은 걸 현재노드로 하고 열린리스트에서 닫힌 리스트로 옮기기
		CurNode = OpenList[0];
		for (int i = 0; i < OpenList.Num(); i++)
		{
			if (OpenList[i]->Cost <= CurNode->Cost)
			{
				CurNode = OpenList[i];
			}

		}

		OpenList.Remove(CurNode);
		ClosedList.Add(CurNode);
		if (!OpenCostList.Contains(CurNode))
		{
			OpenCostList.Add(CurNode);
		}

		// 우상,좌상,좌하,우하
		OpneCostListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y + 1, Stamina);
		OpneCostListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y + 1, Stamina);
		OpneCostListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y - 1, Stamina);
		OpneCostListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y - 1, Stamina);

		//상 우 하 좌
		OpneCostListAdd(CurNode->GridIndex.X, CurNode->GridIndex.Y + 1, Stamina);
		OpneCostListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y, Stamina);
		OpneCostListAdd(CurNode->GridIndex.X, CurNode->GridIndex.Y - 1, Stamina);
		OpneCostListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y, Stamina);
	}
	for (int32 i = 0; i < OpenCostList.Num(); i++)
	{
		//OpenCostList[i]->ChangeGridPathState(EPathState::PATH);
	}

}

void UAstarActorComponent::PathFindingNearCost(ACharacterBase* InputCharacter)
{

}

void UAstarActorComponent::PathFindingInArea()
{

}

void UAstarActorComponent::OpneListAdd(int32 CheckX, int32 CheckY)
{
	//UE_LOG(LogTemp, Log, TEXT("Grid  x: %d, y: %d"), GridX, GridY);

	bool allowDiagonal = false;
	bool dontCrossCornor = true;
	if (CheckX >= 0 && CheckX < GridX && CheckY >= 0 && CheckY < GridY &&
		(GridArray[CheckX][CheckY]->GridState != EGridState::INVALID) && !ClosedList.Contains(GridArray[CheckX][CheckY])&&GridArray[CheckX][CheckY]->GetOnCharacter()==nullptr)
	{
		//대각선 허용시 , 벽 사이로 통과 안됨.
		if (allowDiagonal) {
			if (GridArray[CurNode->GridIndex.X][CheckY]->GetTileState() == EGridState::INVALID
				&& GridArray[CheckX][CurNode->GridIndex.Y]->GetTileState() == EGridState::INVALID)
			{
				return;
			}
		}
		if (dontCrossCornor)
		{
			if (GridArray[CurNode->GridIndex.X][CheckY]->GetTileState() == EGridState::INVALID
				|| GridArray[CheckX][CurNode->GridIndex.Y]->GetTileState() == EGridState::INVALID)
			{
				return;
			}
		}

		//이웃노드에 넣고, 직선은 10, 대각선은 14비용
		TObjectPtr<AGrid> NeighborNode = GridArray[CheckX][CheckY];
		int MoveCost = CurNode->Cost + (CurNode->GridIndex.X - CheckX == 0 || CurNode->GridIndex.Y - CheckY == 0 ? 10 : 14);

		//이동비용이 이웃노드G보다 작거나 또는 열린리스트에 이웃노드가 없다면 G,H, ParentNode를 설정 후 열린리스트에 추가
		if (MoveCost < NeighborNode->Cost || !OpenList.Contains(NeighborNode))
		{
			NeighborNode->Cost = MoveCost;
			NeighborNode->Heuristic = (FMath::Abs(NeighborNode->GridIndex.X - TargetNode->GridIndex.X) +
				FMath::Abs(NeighborNode->GridIndex.Y - TargetNode->GridIndex.Y)) * 10;
			NeighborNode->ParentNode = CurNode;
			OpenList.Add(NeighborNode);
		}
	}
}

void UAstarActorComponent::OpneCostListAdd(int32 CheckX, int32 CheckY,int32 Stamina)
{
	bool allowDiagonal = false;
	bool dontCrossCornor = false;
	if (CheckX == 0 && CheckY == 0)
	{

	}
	if (CheckX >= 0 && CheckX < GridX && CheckY >= 0 && CheckY < GridY && !ClosedList.Contains(GridArray[CheckX][CheckY]))
	{
		//대각선 허용시 , 벽 사이로 통과 안됨.
		if (allowDiagonal) {
			if (GridArray[CurNode->GridIndex.X][CheckY]->GetTileState() == EGridState::INVALID
				&& GridArray[CheckX][CurNode->GridIndex.Y]->GetTileState() == EGridState::INVALID)
			{
				return;
			}
		}
		if (dontCrossCornor)
		{
			if (GridArray[CurNode->GridIndex.X][CheckY]->GetTileState() == EGridState::INVALID
				|| GridArray[CheckX][CurNode->GridIndex.Y]->GetTileState() == EGridState::INVALID)
			{
				return;
			}
		}

		//이웃노드에 넣고, 직선은 10, 대각선은 14비용
		TObjectPtr<AGrid> NeighborNode = GridArray[CheckX][CheckY];
		int MoveCost = CurNode->Cost + (CurNode->GridIndex.X - CheckX == 0 || CurNode->GridIndex.Y - CheckY == 0 ? 10 : 14);
		if (NeighborNode->GetTileState() != EGridState::NOTUSED && NeighborNode->GetTileState() != EGridState::INVALID)
		{
			if (!OpenList.Contains(NeighborNode) && MoveCost <= Stamina)
			{

				NeighborNode->Cost = MoveCost;


				OpenList.Add(NeighborNode);
			}
		}
	}
}

void UAstarActorComponent::InitAstarActor(int32 x, int32 y, TArray<TArray<TObjectPtr<class AGrid>>> Array)
{
	GridArray = Array;
	GridX = x;
	GridY = y;
}

void UAstarActorComponent::SetGridManager(AGridManager* gm)
{
	GridManager = gm;
	GridArray = GridManager->GetGridArray();
}

AGridManager* UAstarActorComponent::GetGridManager()
{
	return GridManager;
}

void UAstarActorComponent::FindNearGridByNumber(AGrid* StartGrid, int32 TargetNumber, TArray<class AGrid*>& OutputGrids)
{
	OutputGrids.Empty();

	GridX = GridManager->GetGridX();
	GridY = GridManager->GetGridY();
	TArray<AGrid*>TempOpenList = TArray<TObjectPtr<AGrid>>();

	TempOpenList.Add(StartGrid);

	for (int32 i = 1; i <=TargetNumber; i++)
	{
		// 우상,좌상,좌하,우하
		OpenListAddByNumber(StartGrid->GridIndex.X + i, StartGrid->GridIndex.Y + i, TempOpenList);
		OpenListAddByNumber(StartGrid->GridIndex.X - i, StartGrid->GridIndex.Y + i, TempOpenList);
		OpenListAddByNumber(StartGrid->GridIndex.X - i, StartGrid->GridIndex.Y - i, TempOpenList);
		OpenListAddByNumber(StartGrid->GridIndex.X + i, StartGrid->GridIndex.Y - i, TempOpenList);

		//상 우 하 좌
		OpenListAddByNumber(StartGrid->GridIndex.X, StartGrid->GridIndex.Y + i, TempOpenList);
		OpenListAddByNumber(StartGrid->GridIndex.X + i, StartGrid->GridIndex.Y, TempOpenList);
		OpenListAddByNumber(StartGrid->GridIndex.X, StartGrid->GridIndex.Y - i, TempOpenList);
		OpenListAddByNumber(StartGrid->GridIndex.X - i, StartGrid->GridIndex.Y, TempOpenList);
	}
	OutputGrids = TempOpenList;
}

void UAstarActorComponent::OpenListAddByNumber(int32 CheckX, int32 CheckY, TArray<class AGrid*>& TempGrid)
{

	if (CheckX >= 0 && CheckX < GridX && CheckY >= 0 && CheckY < GridY &&
		(GridArray[CheckX][CheckY]->GridState != EGridState::INVALID) )
	{

		TObjectPtr<AGrid> NeighborNode = GridArray[CheckX][CheckY];
	

		if (!TempGrid.Contains(NeighborNode))
		{
			TempGrid.Add(NeighborNode);
		}
	}
}

void UAstarActorComponent::FindGridAreaByNumber(AGrid* StartGrid, int32 TargetNumber, TArray<class AGrid*>& OutputGrids)
{
	OutputGrids.Empty();

	GridX = GridManager->GetGridX();
	GridY = GridManager->GetGridY();
	TArray<AGrid*>TempOpenList = TArray<TObjectPtr<AGrid>>();

	TempOpenList.Add(StartGrid);


	for (int32 i = 0; i <= TargetNumber; i++)
	{

	
			for (int32 up = 0; up <= TargetNumber; up++)
			{
				OpenListAddByNumber(StartGrid->GridIndex.X+i, StartGrid->GridIndex.Y + up, TempOpenList);
			}
			for (int32 down = 0; down <= TargetNumber; down++)
			{
				OpenListAddByNumber(StartGrid->GridIndex.X+i, StartGrid->GridIndex.Y - down, TempOpenList);
			}
			for (int32 up = 0; up <= TargetNumber; up++)
			{
				OpenListAddByNumber(StartGrid->GridIndex.X - i, StartGrid->GridIndex.Y + up, TempOpenList);
			}
			for (int32 down = 0; down <= TargetNumber; down++)
			{
				OpenListAddByNumber(StartGrid->GridIndex.X - i, StartGrid->GridIndex.Y - down, TempOpenList);
			}
		
	
	}

	
	OutputGrids = TempOpenList;

}

