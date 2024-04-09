// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GridManager.h"
#include "Grid.h"
#include <Components/StaticMeshComponent.h>
#include "Algo/Reverse.h"
#include "UObject/ConstructorHelpers.h"
#include "AStar/AstarActorComponent.h"
#include "ProceduralMeshComponent.h"
#include "Data/Enum/Enums.h"
#include "Character/CharacterBase.h"

// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//GridX = 30;
	//GridY = 35;

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempGridMesh(TEXT("/Script/Engine.StaticMesh'/Game/Saga/Meshs/Grid/Flat.Flat'"));
	StoredGridMesh = TempGridMesh.Object;
	ConstructorHelpers::FObjectFinder<UMaterialInterface> TempGridMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Saga/Materials/Grid/MI_GridOutLineMaterial.MI_GridOutLineMaterial'"));
	StoredGridMaterial = TempGridMaterial.Object;

	AstarCompo = CreateDefaultSubobject<UAstarActorComponent>(TEXT("AstarComponent"));

}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();

	//FVector Center = AABB.GetCenter();
	//FVector Extent = AABB.GetExtent();

	//UE_LOG(LogTemp, Log, TEXT("Center: %f,%f"), Center.X, Center.Y);
	//UE_LOG(LogTemp, Log, TEXT("Extent: %f,%f"), Extent.X, Extent.Y);


	//// 시각적으로 FBox를 디버깅용으로 표시
	//DrawDebugBox(GetWorld(), Center, Extent, FColor::Green, false, -1.f, 0, 10.f);
}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FIntPoint AGridManager::LocationToTile(FVector location)
{
	float ReturnRow;
	float ReturnCol;
	FVector MyLocation = GetActorLocation();

	//UE_LOG(LogTemp, Log, TEXT("Grid x : %d, y:%d"), GridX, GridY);

	ReturnRow = (location.X - MyLocation.X);

	ReturnRow = ReturnRow / (GridX * TileSize); 
	//UE_LOG(LogTemp, Log, TEXT("GridWidth : %f"), (GridX * TileSize));
	//UE_LOG(LogTemp, Log, TEXT("ReturnRow : %f"), ReturnRow);
	ReturnRow = ReturnRow * GridX;
	//UE_LOG(LogTemp, Log, TEXT("ReturnRow : %f"), ReturnRow);
	//UE_LOG(LogTemp, Log, TEXT("GridX : %f"), GridX);


	ReturnCol = (location.Y - MyLocation.Y);
	ReturnCol = ReturnCol / (GridY * TileSize);
	ReturnCol = ReturnCol * GridY;

	float ReturnX;
	float ReturnY;
	ReturnX = location.X - MyLocation.X;
	ReturnY = location.Y - MyLocation.Y;



	FIntPoint ReturnPoint = { (int)ReturnRow,(int)ReturnCol };
	return ReturnPoint;
}

FVector AGridManager::TileToGridLocation(int32 Row, int32 Col, bool IsCenter)
{
	float ReturnX;
	float ReturnY;
	FVector GridLocation = GetActorLocation();

	ReturnX = (Row * TileSize) + GridLocation.X;
	ReturnY = (Col * TileSize) + GridLocation.Y;

	if (IsCenter)
	{
		ReturnX += (TileSize / 2);
		ReturnY += (TileSize / 2);
	}

	return FVector(ReturnX, ReturnY, 0);
}

void AGridManager::CheckOvelap()
{
	TArray<FOverlapResult> Overlaps;
	FVector OverlapLocation = GridArray[GridX / 2][GridY / 2]->GetActorLocation();
	FCollisionShape OverlapShape = FCollisionShape::MakeSphere(TileSize*(GridX/2-1)); // 충돌 형태 (구 형태의 오버랩)
	bool bOverlap = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		OverlapLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECC_GameTraceChannel1), // 충돌 객체 타입
		OverlapShape
	);

	if (bOverlap)
	{

		for (const FOverlapResult& Overlap : Overlaps)
		{
			AGrid* OverlappingGrid = Cast<AGrid>(Overlap.GetActor());
			if (OverlappingGrid)
			{
				FIntPoint xy = OverlappingGrid->GetGridIndex();
				//UE_LOG(LogTemp, Log, TEXT("x : %d, y :%d"),xy.X,xy.Y);
				GridOnOffDelegate.Broadcast(xy, true);
				GridStateDelegate.Broadcast(xy, EGridState::VALID);
				GridViewOnOffDelegate.AddUObject(OverlappingGrid, &AGrid::GridViewOnOff);
				OverlappingGrid->CheckOverlap();
			}
		}
	}
	else
	{
		// 오버랩 없음
	}
	DrawDebugSphere(
	GetWorld(),
	OverlapLocation,
	(TileSize * (GridX / 2)), // 스피어 반지름
	1, // 스피어 분할 수 (세밀함 조절)
	FColor::Green,
	true, // 지속적으로 그리기 여부
	-1, // 지속 시간
	0.1, // 라인 두께
	1 // 디버그 뷰포트 표시 우선순위
);
}

UAstarActorComponent* AGridManager::GetAstartCompo()
{
	return AstarCompo;
}

void AGridManager::GeneratedGrid(int32 NumX, int32 NumY)
{
	GridX = NumX;
	GridY = NumY;


	GridArray.SetNumZeroed(GridX);
	for (int32 i = 0; i < GridArray.Num(); ++i)
	{
		GridArray[i].SetNumZeroed(GridY);
	}

	for (int32 y = 0; y < GridY; ++y)
	{
		for (int32 x = 0; x < GridX; ++x)
		{
			const float xPos = ((x * TileSize) + ((x + 1) * TileSize)) / 2;
			const float yPos = ((y * TileSize) + ((y + 1) * TileSize)) / 2;

			TObjectPtr<AGrid> newGrid = GetWorld()->SpawnActor<AGrid>(AGrid::StaticClass(), FVector(FIntPoint(xPos, yPos), 0.1f), FRotator::ZeroRotator);
			newGrid->GridIndex = FIntPoint(x, y);
			GridArray[x][y] = newGrid;
			GridArray[x][y]->InitBoth(StoredGridMesh, StoredGridMaterial);
			GridArray[x][y]->SetGridManager(this);
			//newGrid->ChangeGridMeshColor(FLinearColor(1, 1, 0));
			//GridArray[x][y]->CheckOverlap();
			GridOnOffDelegate.Broadcast(FIntPoint(x,y),false);
			//GridStateDelegate.Broadcast(FIntPoint(x, y), EGridState::INVALID);

		}
	}
	if (AstarCompo)
	{
		AstarCompo->SetGridManager(this);
	}
	else
	{
		AstarCompo = CreateDefaultSubobject<UAstarActorComponent>(TEXT("AstarComponent"));
		AstarCompo->SetGridManager(this);
	}
	for (int32 y = 0; y < GridY; ++y)
	{
		for (int32 x = 0; x < GridX; ++x)
		{
			GridFindNeighbors(GridArray[x][y]);
		}
	}
	CheckOvelap();
	//TArray<AGrid*> tempGrids;
	//AstarCompo->FindGridAreaByNumber(GridArray[8][8], 1, tempGrids);

	//for (AGrid* tgrid : tempGrids)
	//{
	//	tgrid->ChangeMaterialColor_Implementation(EGridColor::YELLO);
	//}	
	//AstarCompo->FindGridAreaByNumber(GridArray[8][8], 3, tempGrids);
	
	//TArray<AGrid*> tempGrids;
	//tempGrids = GetDiagonalGrids(FIntPoint(8, 8), 1, 30);
	//tempGrids = GetLineGrids(FIntPoint(8, 8), 1, 3);

	//for (AGrid* tgrid : tempGrids)
	//{
	//	tgrid->ChangeMaterialColor_Implementation(EGridColor::YELLO);
	//	tgrid->bIsCandidateTarget = true;
	//}
}

void AGridManager::GeneratedGridArea(int32 NumX, int32 NumY, float XPos, float YPos, float Radius)
{

	GridX = NumX;
	GridY = NumY;


	GridArray.SetNumZeroed(GridX);
	for (int32 i = 0; i < GridArray.Num(); ++i)
	{
		GridArray[i].SetNumZeroed(GridY);
	}

	for (int32 y = 0; y < GridY; ++y)
	{
		for (int32 x = 0; x < GridX; ++x)
		{
			const float xPos = ((x * TileSize) + ((x + 1) * TileSize)) / 2;
			const float yPos = ((y * TileSize) + ((y + 1) * TileSize)) / 2;

			TObjectPtr<AGrid> newGrid = GetWorld()->SpawnActor<AGrid>(AGrid::StaticClass(), FVector(FIntPoint(xPos, yPos), 0.1f), FRotator::ZeroRotator);
			newGrid->GridIndex = FIntPoint(x, y);
			GridArray[x][y] = newGrid;
			GridArray[x][y]->InitBoth(StoredGridMesh, StoredGridMaterial);
			GridArray[x][y]->SetGridManager(this);
			//GridArray[x][y]->CheckOverlap();
		}
	}

	if (AstarCompo)
	{
		AstarCompo->SetGridManager(this);
	}
	else
	{
		AstarCompo = CreateDefaultSubobject<UAstarActorComponent>(TEXT("AstarComponent"));
		AstarCompo->SetGridManager(this);
	}
	AABB_BottomLeftCorner = GridArray[0][0]->GetActorLocation();
	AABB_TopRightCorner = GridArray[GridX - 1][GridY - 1]->GetActorLocation();
	//AABB=FBox(AABB_BottomLeftCorner, AABB_TopRightCorner);
	AABB.Min = AABB_BottomLeftCorner;
	AABB.Max = AABB_TopRightCorner;

	UE_LOG(LogTemp, Log, TEXT("BLC: %f,%f"), AABB_BottomLeftCorner.X, AABB_BottomLeftCorner.Y);
	UE_LOG(LogTemp, Log, TEXT("BLC: %f,%f"), AABB_TopRightCorner.X, AABB_TopRightCorner.Y);

}

bool AGridManager::IsInsideAABB(FVector Location)
{
	bool IsInside;

	IsInside = AABB.IsInsideXY(Location);

	return IsInside;
}

void AGridManager::StartPathFind()
{
	//AstarCompo->PathFinding();
	AstarCompo->PathFindingNearCost(68);
}

void AGridManager::StartPathFind(ACharacterBase* InputCharacter, int32 Stamina)
{
	AstarCompo->PathFindingNearCost(Stamina);

}

void AGridManager::StartPathFind(AGrid* StartGrid)
{
	if (StartGrid != nullptr && StartGrid->OnCharacter != nullptr)
	{
		AstarCompo->PathFindingNearCost(StartGrid);
	}
}

void AGridManager::StartPathFind(ACharacterBase* InputCharacter)
{
	AstarCompo->PathFindingNearCost(InputCharacter->GetCharacterGrid());

}

void AGridManager::CharacterGetPathArea(AGrid* CharacterOnGrid)
{
	CharacterOnGrid->OnCharacter->SetCharacterArea(AstarCompo->GetPathArea(CharacterOnGrid));
}

void AGridManager::SpawnOnGrid(AGrid* TargetGrid, FCharacterStuructData InputData)
{
	FIntPoint TargetGridIndex = TargetGrid->GetGridIndex_Implementation();
	GridArray[TargetGridIndex.X][TargetGridIndex.Y]->SetOnCharacter(InputData);
}

void AGridManager::ShowGridArea(AGrid* TargetGrid)
{
	if (TargetGrid->OnCharacter)
	{

	}

}

void AGridManager::ClearAllGrids()
{
	GridColorDelegate.Broadcast(EGridColor::WHITE);
	//타겟 초기화

}

void AGridManager::ClearPathStateGrids()
{	//그리드의 PATH 상태를 원래대로 돌림
	GridPathClearDelegate.Broadcast();
}

void AGridManager::ClearSelectedGrids(TArray<AGrid*> InGrids)
{
	if (InGrids.Num() > 0)
	{
		for (AGrid* ElGrid : InGrids)
		{
			ElGrid->GridStateClear();
			ElGrid->bIsCandidateTarget = false;
		}
	}

}

void AGridManager::ChangeGridInformation(AGrid* PreGrid, AGrid* NewGrid)
{
	if (PreGrid == nullptr || NewGrid == nullptr)
	{
		return;
	}
	if (PreGrid->GetOnCharacter())
	{
		ACharacterBase* TempCharacter;
		TempCharacter = PreGrid->OnCharacter;
		PreGrid->OnCharacter = nullptr;
		TempCharacter->SetCharacterGrid(NewGrid); // Include Grid Setting
	}
	NewGrid->GridState = PreGrid->GridState;
	PreGrid->GridState = EGridState::VALID;

}

void AGridManager::ChangeGridInformation(AGrid* PreGrid, AGrid* NewGrid, ACharacterBase* InCharacter)
{


}

void AGridManager::RefreshGrids()
{
	GridRefreshDelegate.Broadcast();
}

TArray<class AGrid*> AGridManager::FindCharacterPath(ACharacterBase* Character)
{
	//캐릭터 이동범위 찾는함수

	return AstarCompo->GetPathArea(Character);
}

TArray<class AGrid*> AGridManager::FindCharacterPath(AGrid* CharacterUnderGrid)
{	//캐릭터 이동범위 찾는함수


	return  AstarCompo->GetPathArea(CharacterUnderGrid);
}

bool AGridManager::CheckInvalidGrid(FIntPoint InputXY)
{	//그리드가 (총) 범위 벗어났는지 확인

	return InputXY.X >= 0 && InputXY.X < GridX && InputXY.Y >= 0 && InputXY.Y < GridY;

}



TArray<class AGrid*> AGridManager::GetSpellRangeGrid(FIntPoint StartPoint, int32 Min, int32 Max, ESpellGridRangeTypeEnum SpellType)
{
	TArray<AGrid*> RetGrids;

	switch (SpellType)
	{
	case ESpellGridRangeTypeEnum::DIAGONAL:
		RetGrids = GetDiagonalGrids(StartPoint, Min, Max);
		break;
	case ESpellGridRangeTypeEnum::SQUARE:
		RetGrids = GetSquereGrids(StartPoint, Min, Max);
		break;
	case ESpellGridRangeTypeEnum::LINE:
		RetGrids = GetLineGrids(StartPoint, Min, Max);
		break;
	case ESpellGridRangeTypeEnum::MAX:
		break;
	default:
		break;
	}

	return RetGrids;
}

TArray<class AGrid*> AGridManager::GetSpellRangeGrid(ACharacterBase* Character, FCharacterCombatData SpellData)
{
	TArray<AGrid*> RetGrids;
	FIntPoint StartPoint = Character->GetCharacterGrid()->GetGridIndex_Implementation();
	int32 Min = SpellData.D_RangeMin;
	int32 Max = SpellData.D_RangeMax;
	switch (SpellData.D_SpellGridShapeType)
	{
	case ESpellGridRangeTypeEnum::DIAGONAL:
		RetGrids = GetDiagonalGrids(StartPoint, Min, Max);
		break;
	case ESpellGridRangeTypeEnum::SQUARE:
		RetGrids = GetSquereGrids(StartPoint, Min, Max);
		break;
	case ESpellGridRangeTypeEnum::LINE:
		RetGrids = GetLineGrids(StartPoint, Min, Max);
		break;
	case ESpellGridRangeTypeEnum::MAX:
		break;
	default:
		break;
	}

	return RetGrids;
}

TArray<class AGrid*> AGridManager::GetSpellRangeGrid(AGrid* StartGrid, FCharacterCombatData SpellData)
{
	TArray<AGrid*> RetGrids;
	FIntPoint StartPoint = StartGrid->GetGridIndex_Implementation();
	int32 Min = SpellData.D_RangeMin;
	int32 Max = SpellData.D_RangeMax;

	switch (SpellData.D_SpellGridShapeType)
	{
	case ESpellGridRangeTypeEnum::DIAGONAL:
		RetGrids = GetDiagonalGrids(StartPoint, Min, Max);
		break;
	case ESpellGridRangeTypeEnum::SQUARE:
		RetGrids = GetSquereGrids(StartPoint, Min, Max);
		break;
	case ESpellGridRangeTypeEnum::LINE:
		RetGrids = GetLineGrids(StartPoint, Min, Max);
		break;
	case ESpellGridRangeTypeEnum::MAX:
		break;
	default:
		break;
	}

	return RetGrids;
}

TArray<class AGrid*> AGridManager::GetDiagonalGrids(FIntPoint StartPoint, int32 Min, int32 Max)
{
	int32 Sx = StartPoint.X;
	int32 Sy = StartPoint.Y;
	TArray<AGrid*> RetGrids;
	for (int32 i = Min; i < Max; i++)
	{
		for (int32 j = Min; j < Max; j++)
		{
			if (i == j)
			{

				if (CheckInvalidGrid(FIntPoint(Sx - i, Sy - j)))
				{
					//좌 상
					if (!RetGrids.Contains(GridArray[Sx - i][Sy - j])&& GridArray[Sx - i][Sy - j]->GridState != EGridState::INVALID)
					RetGrids.Add(GridArray[Sx - i][Sy - j]);
				}
				if (CheckInvalidGrid(FIntPoint(Sx - i, Sy + j)))
				{
					//좌 하
					if (!RetGrids.Contains(GridArray[Sx - i][Sy + j]) && GridArray[Sx - i][Sy + j]->GridState != EGridState::INVALID)
					RetGrids.Add(GridArray[Sx - i][Sy + j]);

				}
				if (CheckInvalidGrid(FIntPoint(Sx + i, Sy + j)))
				{
					//우 상
					if (!RetGrids.Contains(GridArray[Sx + i][Sy + j]) && GridArray[Sx + i][Sy + j]->GridState != EGridState::INVALID)
					RetGrids.Add(GridArray[Sx + i][Sy + j]);

				}
				if (CheckInvalidGrid(FIntPoint(Sx + i, Sy - j)))
				{
					//우 하
					if (!RetGrids.Contains(GridArray[Sx + i][Sy - j]) && GridArray[Sx + i][Sy - j]->GridState != EGridState::INVALID)
					RetGrids.Add(GridArray[Sx + i][Sy - j]);

				}
			}
		}
	}


	return RetGrids;
}

TArray<class AGrid*> AGridManager::GetSquereGrids(FIntPoint StartPoint, int32 Min, int32 Max)
{
	int32 Sx = StartPoint.X;
	int32 Sy = StartPoint.Y;
	TArray<AGrid*> RetGrids;


	for (int32 i = -Max; i <= Max; i++)
	{
		for (int32 j = -Max; j <= Max; j++)
		{
			if (FMath::Abs(i) > Min || FMath::Abs(j) > Min)
			{
				if (CheckInvalidGrid(FIntPoint(Sx + i, Sy + j)))
				{
					if (!RetGrids.Contains(GridArray[Sx + i][Sy + j]))
					{
						if (GridArray[Sx + i][Sy + j]->GridState != EGridState::INVALID)
						RetGrids.Add(GridArray[Sx + i][Sy + j]);
					}
				}
			}
		}
	}

	return RetGrids;
}

TArray<class AGrid*> AGridManager::GetLineGrids(FIntPoint StartPoint, int32 Min, int32 Max)
{
	int32 Sx = StartPoint.X;
	int32 Sy = StartPoint.Y;
	TArray<AGrid*> RetGrids;

	for (int32 i = Min; i < Max; i++)
	{
		for (int32 j = Min; j < Max; j++)
		{
			
				if (CheckInvalidGrid(FIntPoint(Sx-i, Sy)))
				{
					// 좌
					if(GridArray[Sx - i][Sy]->GridState!=EGridState::INVALID)
					RetGrids.Add(GridArray[Sx - i][Sy]);
				}
				if (CheckInvalidGrid(FIntPoint(Sx, Sy + j)))
				{
					// 상
					if (GridArray[Sx][Sy + j]->GridState != EGridState::INVALID)
					RetGrids.Add(GridArray[Sx][Sy + j]);

				}
				if (CheckInvalidGrid(FIntPoint(Sx + i, Sy )))
				{
					//우
					if (GridArray[Sx + i][Sy]->GridState != EGridState::INVALID)
					RetGrids.Add(GridArray[Sx + i][Sy ]);

				}
				if (CheckInvalidGrid(FIntPoint(Sx, Sy - j)))
				{
					//하
					if (GridArray[Sx][Sy - j]->GridState != EGridState::INVALID)
					RetGrids.Add(GridArray[Sx][Sy - j]);

				}
			
		}
	}




	return RetGrids;
}

TArray<class AGrid*> AGridManager::FindBFS(AGrid* StartGrid)
{
	TArray<AGrid*> RetGrids;

	TQueue<AGrid*> BfsQueue;
	bool ContainsDiagonal = false;
	BfsQueue.Enqueue(StartGrid);
	
	while (!BfsQueue.IsEmpty())
	{
		AGrid* CurrentGrid = *(BfsQueue.Peek());
		BfsQueue.Pop();
	/*	if (CurrentGrid->bIsCandidateTarget == true && CurrentGrid != StartGrid)
		{
			RetGrids.Add(CurrentGrid);
		}*/

		for (const auto& Elem : CurrentGrid->GridNeighborsMap)
		{
			EGridDirection GDir = Elem.Key;
			 AGrid* InGrid = Elem.Value;
			 if (ContainsDiagonal == true)
			 {
				 if (RetGrids.Contains(InGrid) == false)
				 {
					 if (InGrid->bIsCandidateTarget == true)
					 {
						 BfsQueue.Enqueue(InGrid);
						 RetGrids.Add(InGrid);
					 }
				 }
			 }
			 else
			 {
				 if (GDir == EGridDirection::NorthEast || GDir == EGridDirection::NorthWest || 
					 GDir == EGridDirection::SouthEast || GDir == EGridDirection::SouthWest)
				 {
					 continue;
				 }

				 if (RetGrids.Contains(InGrid) == false)
				 {
					 if (InGrid->bIsCandidateTarget == true)
					 {
						 BfsQueue.Enqueue(InGrid);
						 RetGrids.Add(InGrid);
					 }
				 }
			 }
			 
		}

	}
	for (AGrid* ingrid : RetGrids)
	{
		DrawDebugSphere(GetWorld(), ingrid->GetGridLocation_Implementation(), 10.0f, 16, FColor::Green, false, 3, 0, 1);
	}

	return RetGrids;
}

TArray<class AGrid*> AGridManager::FindBFS(AGrid* StartGrid, int32 TargetNum)
{

	TArray<AGrid*> RetGrids;
	TQueue<AGrid*> BfsQueue;
	bool ContainsDiagonal = false;
	BfsQueue.Enqueue(StartGrid);
	bool InturupBreak = false;


	RetGrids.Add(StartGrid);
	while (!BfsQueue.IsEmpty())
	{
		AGrid* CurrentGrid = *(BfsQueue.Peek());
		BfsQueue.Pop();
		/*	if (CurrentGrid->bIsCandidateTarget == true && CurrentGrid != StartGrid)
			{
				RetGrids.Add(CurrentGrid);
			}*/

		for (const auto& Elem : CurrentGrid->GridNeighborsMap)
		{
			EGridDirection GDir = Elem.Key;
			AGrid* InGrid = Elem.Value;
			if (ContainsDiagonal == true)
			{
				if (RetGrids.Contains(InGrid) == false)
				{
					if (InGrid->bIsCandidateTarget == true)
					{
						
						if (RetGrids.Num() == TargetNum)
						{

							InturupBreak = true;
							break;
						}
						BfsQueue.Enqueue(InGrid);
						RetGrids.Add(InGrid);
					}
				}
			}
			else
			{
				if (GDir == EGridDirection::NorthEast || GDir == EGridDirection::NorthWest ||
					GDir == EGridDirection::SouthEast || GDir == EGridDirection::SouthWest)
				{
					continue;
				}

				if (RetGrids.Contains(InGrid) == false)
				{
					if (InGrid->bIsCandidateTarget == true)
					{
					
						if (RetGrids.Num() == TargetNum)
						{
							InturupBreak = true;
							break;
						}
						BfsQueue.Enqueue(InGrid);
						RetGrids.Add(InGrid);
					}
				}
			}

		}
		if (InturupBreak==true)
		{
			BfsQueue.Empty();
			break;
		}

	}

	return RetGrids;
}

TArray<class AGrid*> AGridManager::FindBFS(AGrid* StartGrid, FCharacterCombatData SpellData)
{
	if (SpellData.bIsSingleAction == true)
	{
		
		return FindBFS(StartGrid, 1);

	}
	else
	{

		TArray<AGrid*> RetGrids;
		TQueue<AGrid*> BfsQueue;
		bool ContainsDiagonal = false;
		BfsQueue.Enqueue(StartGrid);
		bool InturupBreak = false;
		int TargetNum = SpellData.D_TargetNum;

		if (SpellData.D_SpellGridShapeType == ESpellGridRangeTypeEnum::DIAGONAL)
		{
			 ContainsDiagonal = true;
		}

		RetGrids.Add(StartGrid);
		while (!BfsQueue.IsEmpty())
		{
			AGrid* CurrentGrid = *(BfsQueue.Peek());
			BfsQueue.Pop();
			/*	if (CurrentGrid->bIsCandidateTarget == true && CurrentGrid != StartGrid)
				{
					RetGrids.Add(CurrentGrid);
				}*/

			for (const auto& Elem : CurrentGrid->GridNeighborsMap)
			{
				EGridDirection GDir = Elem.Key;
				AGrid* InGrid = Elem.Value;
				if (ContainsDiagonal == true)
				{
					if (RetGrids.Contains(InGrid) == false)
					{
						if (InGrid->bIsCandidateTarget == true)
						{

							if (RetGrids.Num() == TargetNum)
							{

								InturupBreak = true;
								break;
							}
							BfsQueue.Enqueue(InGrid);
							RetGrids.Add(InGrid);
						}
					}
				}
				else
				{
					if (GDir == EGridDirection::NorthEast || GDir == EGridDirection::NorthWest ||
						GDir == EGridDirection::SouthEast || GDir == EGridDirection::SouthWest)
					{
						continue;
					}

					if (RetGrids.Contains(InGrid) == false)
					{
						if (InGrid->bIsCandidateTarget == true)
						{

							if (RetGrids.Num() == TargetNum)
							{
								InturupBreak = true;
								break;
							}
							BfsQueue.Enqueue(InGrid);
							RetGrids.Add(InGrid);
						}
					}
				}

			}
			if (InturupBreak == true)
			{
				BfsQueue.Empty();
				break;
			}

		}

		return RetGrids;

	}

}

TArray<class AGrid*> AGridManager::FindBFS(AGrid* StartGrid, FCharacterCombatData SpellData, TArray<AGrid*> ActionGrids)
{
	if (SpellData.bIsSingleAction == true)
	{

		return FindBFS(StartGrid, 1);

	}
	else
	{

		TArray<AGrid*> RetGrids;
		TQueue<AGrid*> BfsQueue;
		bool ContainsDiagonal = false;
		BfsQueue.Enqueue(StartGrid);
		bool InturupBreak = false;
		int32 TargetNum = SpellData.D_TargetNum;
		int32 CheckP = 0;
		if (SpellData.D_SpellGridShapeType == ESpellGridRangeTypeEnum::DIAGONAL)
		{
			ContainsDiagonal = true;
		}

		RetGrids.Add(StartGrid);
		while (!BfsQueue.IsEmpty())
		{
			AGrid* CurrentGrid = *(BfsQueue.Peek());
			BfsQueue.Pop();
			/*	if (CurrentGrid->bIsCandidateTarget == true && CurrentGrid != StartGrid)
				{
					RetGrids.Add(CurrentGrid);
				}*/

			for (const auto& Elem : CurrentGrid->GridNeighborsMap)
			{
				EGridDirection GDir = Elem.Key;
				AGrid* InGrid = Elem.Value;
				if (ContainsDiagonal == true)
				{
					if (RetGrids.Contains(InGrid) == false)
					{
						if (InGrid->bIsCandidateTarget == true)
						{

							if (RetGrids.Num() == TargetNum)
							{

								InturupBreak = true;
								break;
							}
							BfsQueue.Enqueue(InGrid);
							RetGrids.Add(InGrid);
						}
					}
				}
				else
				{
					if (GDir == EGridDirection::NorthEast || GDir == EGridDirection::NorthWest ||
						GDir == EGridDirection::SouthEast || GDir == EGridDirection::SouthWest)
					{
						continue;
					}

					if (RetGrids.Contains(InGrid) == false)
					{
						
						if (ActionGrids.Contains(InGrid) == true)
						{
							CheckP++;
							if (CheckP > (ActionGrids.Num()*10))
							{
								InturupBreak = true;

								break;
							}
							if (RetGrids.Num() == TargetNum)
							{
								InturupBreak = true;
								break;
							}
							BfsQueue.Enqueue(InGrid);
							if (InGrid->bIsCandidateTarget == true)
							{
								RetGrids.Add(InGrid);
							}
						}
					}
				}

			}
			if (InturupBreak == true)
			{
				BfsQueue.Empty();
				break;
			}

		}

		return RetGrids;

	}

}

void AGridManager::GridFindNeighbors(AGrid* StartGrid)
{
	int32 SX = StartGrid->GridIndex.X;
	int32 SY = StartGrid->GridIndex.Y;

	if (CheckInvalidGrid(FIntPoint(SX,SY-1)))
	StartGrid->GridNeighborsMap.Add(EGridDirection::North,GridArray[SX][SY-1]);
	if (CheckInvalidGrid(FIntPoint(SX, SY + 1)))
	StartGrid->GridNeighborsMap.Add(EGridDirection::South, GridArray[SX][SY + 1]);
	if (CheckInvalidGrid(FIntPoint(SX+1, SY )))
	StartGrid->GridNeighborsMap.Add(EGridDirection::East, GridArray[SX+1][SY]);
	if (CheckInvalidGrid(FIntPoint(SX-1, SY)))
	StartGrid->GridNeighborsMap.Add(EGridDirection::West, GridArray[SX-1][SY]);
	if (CheckInvalidGrid(FIntPoint(SX+1, SY - 1)))
	StartGrid->GridNeighborsMap.Add(EGridDirection::NorthEast, GridArray[SX+1][SY-1]);
	if (CheckInvalidGrid(FIntPoint(SX-1, SY - 1)))
	StartGrid->GridNeighborsMap.Add(EGridDirection::NorthWest, GridArray[SX-1][SY-1]);
	if (CheckInvalidGrid(FIntPoint(SX+1, SY + 1)))
	StartGrid->GridNeighborsMap.Add(EGridDirection::SouthEast, GridArray[SX+1][SY+1]);
	if (CheckInvalidGrid(FIntPoint(SX-1, SY +1)))
	StartGrid->GridNeighborsMap.Add(EGridDirection::SouthWest, GridArray[SX-1][SY+1]);
}

