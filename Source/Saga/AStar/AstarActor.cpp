// Fill out your copyright notice in the Description page of Project Settings.


#include "AStar/AstarActor.h"
#include "Algo/Reverse.h"
#include "Grid/Grid.h"

// Sets default values
AAstarActor::AAstarActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAstarActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAstarActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAstarActor::PathFinding()
{

	StartNode = GridArray[0][0];
	TargetNode = GridArray[6][6];

	OpenList = TArray<TObjectPtr<AGrid>>();
	ClosedList = TArray<TObjectPtr<AGrid>>();
	FinalNodeList = TArray<TObjectPtr<AGrid>>();

	OpenList.Add(StartNode);

	while (OpenList.Num() > 0)
	{
		// ��������Ʈ �� ���� F�� �۰� F�� ���ٸ� H�� ���� �� ������� �ϰ� ��������Ʈ���� ���� ����Ʈ�� �ű��
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
				UE_LOG(LogTemp, Log, TEXT("%d��°�� x: %d, y: %d"), i, FinalNodeList[i]->GridIndex.X, FinalNodeList[i]->GridIndex.Y);

			}
			return;
		}
		bool allowDiagonal = false;
		// �밢�� ����
		if (allowDiagonal)
		{
			// ���,�»�,����,����
			OpneListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y + 1);
			OpneListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y + 1);
			OpneListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y - 1);
			OpneListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y - 1);
		}
		//�� �� �� ��
		OpneListAdd(CurNode->GridIndex.X, CurNode->GridIndex.Y + 1);
		OpneListAdd(CurNode->GridIndex.X + 1, CurNode->GridIndex.Y);
		OpneListAdd(CurNode->GridIndex.X, CurNode->GridIndex.Y - 1);
		OpneListAdd(CurNode->GridIndex.X - 1, CurNode->GridIndex.Y);
	}
}

void AAstarActor::OpneListAdd(int32 CheckX, int32 CheckY)
{
	bool allowDiagonal = false;
	bool dontCrossCornor = true;
	if (CheckX >= 0 && CheckX < GridX && CheckY >= 0 && CheckY < GridY &&
		(GridArray[CheckX][CheckY]->GridState != EGridState::INVALID) && !ClosedList.Contains(GridArray[CheckX][CheckY]))
	{
		//�밢�� ���� , �� ���̷� ��� �ȵ�.
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

		//�̿���忡 �ְ�, ������ 10, �밢���� 14���
		TObjectPtr<AGrid> NeighborNode = GridArray[CheckX][CheckY];
		int MoveCost = CurNode->Cost + (CurNode->GridIndex.X - CheckX == 0 || CurNode->GridIndex.Y - CheckY == 0 ? 10 : 14);

		//�̵������ �̿����G���� �۰ų� �Ǵ� ��������Ʈ�� �̿���尡 ���ٸ� G,H, ParentNode�� ���� �� ��������Ʈ�� �߰�
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

void AAstarActor::InitAstarActor(int32 x, int32 y, TArray<TArray<TObjectPtr<class AGrid>>> Array)
{
	GridArray = Array;
	GridX = x;
	GridY = y;
}

