// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/Grid.h"
#include "Grid/GridHighlight.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include "Grid/GridViewComponent.h"
#include "Grid/GridManager.h"
#include "Character/CharacterBase.h"

// Sets default values
AGrid::AGrid(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	GridMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridMesh"));
	GridMaterialInterface = CreateDefaultSubobject<UMaterialInterface>(TEXT("Material"));
	GridMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	GridViewComponent = CreateDefaultSubobject<UGridViewComponent>(TEXT("GridView"));
	
	//ConstructorHelpers::FObjectFinder<UStaticMesh> TempTileMesh(TEXT("/Script/Engine.StaticMesh'/Game/Saga/Meshs/Flat.Flat'"));
	//if (TempTileMesh.Succeeded())
	//{
	//	/*FString msg = "debugmsg";
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, *msg);*/
	//	GridMeshComponent->SetStaticMesh(TempTileMesh.Object);
	//}
	//ConstructorHelpers::FObjectFinder<UMaterialInterface> gridMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Saga/Materials/MI_SquareGrid.MI_SquareGrid'"));
	//if (gridMaterial.Object)
	//{
	//	GridMeshComponent->SetMaterial(0, gridMaterial.Object);
	//}

	GridState = EGridState::INVALID;
	PathState = EPathState::DEFAULT;

	GridMeshComponent->SetCollisionObjectType(ECC_GameTraceChannel1);
	bIsTarget = false;
	bIsCandidateTarget = false;
	bIsPathTarget = false;
}

void AGrid::BeginPlay()
{
	Super::BeginPlay();
	//DrawDebugSphere(
	//	GetWorld(),
	//	GetActorLocation(),
	//	50.0f, // 스피어 반지름
	//	1, // 스피어 분할 수 (세밀함 조절)
	//	FColor::Green,
	//	true, // 지속적으로 그리기 여부
	//	-1, // 지속 시간
	//	0.1, // 라인 두께
	//	1 // 디버그 뷰포트 표시 우선순위
	//);
	//CheckOverlap();
	if (OwnerGridManager)
	{
		//OwnerGridManager->GridOnOffDelegate.Add();
		//GridViewComponent->InitViewComponent(OwnerGridManager,this);
	}
	
}

EGridState AGrid::GetTileState()
{
	return GridState;
}

void AGrid::InitMesh(UStaticMesh* mesh)
{
	if (GridMeshComponent != nullptr)
	{
		GridMeshComponent->SetStaticMesh(mesh);
	}
}

void AGrid::InitMaterial(UMaterialInterface* material)
{
	if (GridMeshComponent->GetStaticMesh())
	{
		GridMeshComponent->SetMaterial(0, material);
	}
}

void AGrid::InitBoth(UStaticMesh* mesh, UMaterialInterface* material)
{
	GridState = EGridState::INVALID;
	if (GridMeshComponent != nullptr)
	{
		GridMeshComponent->SetStaticMesh(mesh);
	}
	if (GridMeshComponent->GetStaticMesh())
	{
		GridMeshComponent->SetMaterial(0, material); 
		GridMaterialInterface = material;
	}
}


void AGrid::ChangeMaterialColor(ESelMaterial Sel, FLinearColor NewColor)
{
	
	if (GridMeshComponent)
	{
		
		FLinearColor CurColor;
		if (Sel == ESelMaterial::GRID)
		{
			GridMeshComponent->GetMaterial(0)->GetVectorParameterValue(TEXT("BaseColor"), CurColor);
			if (CurColor == NewColor)
			{
				return;
			}
			UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(GridMaterialInterface->GetMaterial(), this);
			if (DynamicMaterial)
			{
				DynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), NewColor);
				GridMeshComponent->SetMaterial(0, DynamicMaterial);
			}
		}
		if (Sel == ESelMaterial::PATH)
		{
			GridMeshComponent->GetMaterial(0)->GetVectorParameterValue(TEXT("InsideColor"), CurColor);
			if (CurColor == NewColor)
			{
				return;
			}
			UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(GridMaterialInterface->GetMaterial(), this);
			if (DynamicMaterial)
			{
				DynamicMaterial->SetVectorParameterValue(TEXT("InsideColor"), NewColor);
				GridMeshComponent->SetMaterial(0, DynamicMaterial);
			}
		}
	}
}

void AGrid::CheckOverlap()
{
	TArray<FOverlapResult> Overlaps;
	FVector OverlapLocation = FVector(0.0f, 0.0f, 0.0f);
	FCollisionShape OverlapShape = FCollisionShape::MakeSphere(50.0f); // 충돌 형태 (구 형태의 오버랩)
	FCollisionObjectQueryParams ObjectQueryParams(ECC_Pawn);
	bool bOverlap = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		GetActorLocation(),
		FQuat::Identity,
		FCollisionObjectQueryParams(ECC_Pawn), // 충돌 객체 타입
		OverlapShape
	);
	
	if (bOverlap)
	{
		UE_LOG(LogTemp, Log, TEXT("Find!!"));


		for (const FOverlapResult& Overlap : Overlaps)
		{
			/*AActor* OverlappingActor = Overlap.GetActor();
			if (OverlappingActor)
			{
				OnActor = OverlappingActor;
				UE_LOG(LogTemp, Log, TEXT(" %s %d %d"),*OnActor->GetName(),GridIndex.X,GridIndex.Y);

			}*/
			ACharacterBase* OverlappingActor = Cast<ACharacterBase>(Overlap.GetActor());
			if (OverlappingActor)
			{
				OnCharacter = OverlappingActor;
				OnCharacter->SetGridManager(GetGridManager());
				UE_LOG(LogTemp, Log, TEXT(" %s %d %d"),*OnCharacter->GetName(),GridIndex.X,GridIndex.Y);
				
				//OwnerGridManager->CharacterGetPathArea(this);
			}
		}
	}
	else
	{
		// 오버랩 없음
	}

}

void AGrid::ChangeGridMeshColor(FLinearColor NewColor)
{
	if (GridMeshComponent)
	{

		FLinearColor CurColor;

		GridMeshComponent->GetMaterial(0)->GetVectorParameterValue(TEXT("BaseColor"), CurColor);
		if (CurColor == NewColor)
		{
			return;
		}
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(GridMaterialInterface->GetMaterial(), this);
		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), NewColor);
			GridMeshComponent->SetMaterial(0, DynamicMaterial);
		}

	}
	

}

void AGrid::SetGridColor(EGridColor color)
{
	//그리드 내부색깔 변경
	GridViewComponent->ChangeMaterialColor_Implementation(color);
	

}

void AGrid::RefreshGrid()
{	//그리드 색깔만 바꿈
	//GridStateClear();
	if (PathState == EPathState::DEFAULT)
	{
		ChangeGridMeshColor(FLinearColor(1, 1, 1));
	}
	if (PathState == EPathState::PATH)
	{
		ChangeGridMeshColor(FLinearColor(0, 1, 1));
	}
	if (PathState == EPathState::OUTSIDE)
	{
		ChangeGridMeshColor(FLinearColor(0, 1, 1));
	}
	if (PathState == EPathState::TARGET)
	{
		ChangeGridMeshColor(FLinearColor(1, 0, 1));
	}

}

void AGrid::GridStateClear()
{
	//그리드 패스상태를 돌림
	PathState = EPathState::DEFAULT;
	Cost=0;
	Heuristic=0; //H
	F=0;
	G=0;
}

// On Grid Change Delegate
void AGrid::ChangeGridState(FIntPoint Index, EGridState state)
{
	if (GridState == state)
	{
		return;
	}
	if (Index != GridIndex)
	{
		return;
	}
	switch (state)
	{
	case EGridState::VALID:
		OwnerGridManager->GridOnOffDelegate.Broadcast(GridIndex, true);
		break;
	case EGridState::INVALID:
		OwnerGridManager->GridOnOffDelegate.Broadcast(GridIndex, false);
		break;
	case EGridState::FriendlyUnit:
		OwnerGridManager->GridOnOffDelegate.Broadcast(GridIndex, true);
		break;
	case EGridState::EnemyUnit:
		OwnerGridManager->GridOnOffDelegate.Broadcast(GridIndex, true);
		break;
	case EGridState::MAX:
		break;
	default:
		break;
	}
	GridState = state;
}

// On Grid State Change
void AGrid::ChangeGridPathState(EPathState state)
{
	FLinearColor changeColor;
	switch (state)
	{
	case EPathState::START:
		changeColor = FLinearColor(1, 1, 0);
		PathState = EPathState::START;
		break;
	case EPathState::PATH:
		changeColor = FLinearColor(1, 0, 1);
		PathState = EPathState::PATH;
		break;
	case EPathState::TARGET:
		changeColor = FLinearColor(1, 0, 0);
		PathState = EPathState::TARGET;
		break;
	case EPathState::DEFAULT:
		changeColor = FLinearColor(0, 0, 0);
		PathState = EPathState::DEFAULT;
		break;
	case EPathState::MAX:
		break;
	default:
		break;
	}
	ChangeMaterialColor(ESelMaterial::PATH, changeColor);
}

void AGrid::InitMaterialMap()
{
	return;
}

void AGrid::SetGridManager(AGridManager* Gm)
{
	OwnerGridManager = Gm;
	GridViewComponent->InitViewComponent(OwnerGridManager, this);
	OwnerGridManager->GridOnOffDelegate.AddUObject(this, &AGrid::VisualOnOff);
	OwnerGridManager->GridStateDelegate.AddUObject(this, &AGrid::ChangeGridState);
	OwnerGridManager->GridColorDelegate.AddUObject(this, &AGrid::SetGridColor); //그리드 내부색깔 변경
	OwnerGridManager->GridPathClearDelegate.AddUObject(this, &AGrid::GridStateClear);	//그리드 패스상태를 돌림
	OwnerGridManager->GridRefreshDelegate.AddUObject(this, &AGrid::RefreshGrid); //그리드 아웃라인 색깔만 바꿈

}

AGridManager* AGrid::GetGridManager()
{
	return OwnerGridManager;
}

void AGrid::VisualOnOff(FIntPoint Index, bool OnOff)
{
	if (Index ==GridIndex)
	{
		GridMeshComponent->SetVisibility(OnOff);
		GridViewComponent->VisualOnOff(GridIndex, OnOff);
	}
}

void AGrid::SetOnCharacter(FCharacterStuructData InputData)
{
	if (OnCharacter == nullptr)
	{
		if (InputData.D_CharacterClass)
		{
			OnCharacter = GetWorld()->SpawnActor<ACharacterBase>(InputData.D_CharacterClass,GetActorLocation(),FRotator::ZeroRotator);
			OnCharacter->SpawnCharacterInit(this, InputData);
			OnCharacter->SetGridManager(OwnerGridManager);
			OnCharacter->FindCharacterPath();
			if (InputData.D_Type == EPlayerType::PLAYER)
			{
				GridState = EGridState::FriendlyUnit;
			}
			else if (InputData.D_Type == EPlayerType::ENEMY)
			{
				GridState = EGridState::EnemyUnit;
			}
		}
	}

}

void AGrid::DeployGrid()
{
	//OwnerGridManager->ClearAllGrids();
	if (OnCharacter)
	{		
		OnCharacter->DeployGrids();
	}
}

void AGrid::SetOnActor(AActor* actor)
{
	if (OnActor == nullptr)
	{
		OnActor = actor;
	}
}

AActor* AGrid::GetOnActor()
{
	if (OnActor != nullptr)
	{
		return OnActor;
	}
	else
	{
		return nullptr;
	}
}

void AGrid::ResetOnActor()
{
	OnActor = nullptr;
}



ACharacterBase* AGrid::GetOnCharacter()
{
	if (OnCharacter)
	{
		return OnCharacter;
	}
	else
	{
		return nullptr;
	}
}

void AGrid::GridViewOnOff(FIntPoint Index, bool OnOff)
{
	if (Index == GridIndex)
	{
		GridViewComponent->VisualOnOff(GridIndex, OnOff);
	}
	if (Index == FIntPoint(-1,-1))
	{
		GridViewComponent->VisualOnOff(GridIndex, OnOff);
	}
}

void AGrid::CheckOnActorOvelap()
{

	TArray<FOverlapResult> Overlaps;
	FVector OverlapLocation = GetActorLocation();
	FCollisionShape OverlapShape = FCollisionShape::MakeSphere(50); // 충돌 형태 (구 형태의 오버랩)
	bool bOverlap = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		OverlapLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECC_GameTraceChannel1), // 충돌 객체 타입
		OverlapShape
	);

	if (bOverlap)
	{
		UE_LOG(LogTemp, Log, TEXT("Find!!"));


		for (const FOverlapResult& Overlap : Overlaps)
		{
			ACharacterBase* OverlappingGrid = Cast<ACharacterBase>(Overlap.GetActor());
			if (OverlappingGrid)
			{

			}
		}
	}
	else
	{
		// 오버랩 없음
	}
}

void AGrid::ChangeMaterialColor_Implementation(EGridColor color)
{
	if (GridViewComponent)
	{
		GridViewComponent->ChangeMaterialColor_Implementation(color);
	}
}

void AGrid::ChangeColor_Implementation(FLinearColor NewColor)
{
	if (GridMeshComponent)
	{

		FLinearColor CurColor;
		
			GridMeshComponent->GetMaterial(0)->GetVectorParameterValue(TEXT("BaseColor"), CurColor);
			if (CurColor == NewColor)
			{
				return;
			}
			UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(GridMaterialInterface->GetMaterial(), this);
			if (DynamicMaterial)
			{
				DynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), NewColor);
				GridMeshComponent->SetMaterial(0, DynamicMaterial);
			}
	}
}


FVector AGrid::GetGridLocation_Implementation()
{
	return GetActorLocation();
}



FIntPoint AGrid::GetGridIndex_Implementation()
{
	return GridIndex;
}



