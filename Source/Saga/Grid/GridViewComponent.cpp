// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GridViewComponent.h"
#include "Interface/ChangeColorInterface.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include "GridManager.h"
#include "Grid.h"
#include "Data/Enum/Enums.h"

// Sets default values for this component's properties
UGridViewComponent::UGridViewComponent()
{
	GridMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridTileMesh"));
	GridMaterialInterface = CreateDefaultSubobject<UMaterialInterface>(TEXT("Material"));
	//GridMeshComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempTileMesh(TEXT("/Script/Engine.StaticMesh'/Game/Saga/Meshs/Highlight/HighLightFlat.HighLightFlat'"));
	if (TempTileMesh.Succeeded())
	{
		GridMeshComponent->SetStaticMesh(TempTileMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterialInterface> TempMaterialGreen(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Saga/Materials/Grid/MI_GridMaterialGreen.MI_GridMaterialGreen'"));
	if (TempMaterialGreen.Succeeded())
	{
		GridMaterials.Add(EGridColor::GREEN, TempMaterialGreen.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> TempMaterialWhite(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Saga/Materials/Grid/MI_GridMaterialWhite.MI_GridMaterialWhite'"));
	if (TempMaterialWhite.Succeeded())
	{
		GridMaterials.Add(EGridColor::WHITE, TempMaterialWhite.Object);
		GridMeshComponent->SetMaterial(0, TempMaterialWhite.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> TempMaterialRed(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Saga/Materials/Grid/MI_GridMaterialRed.MI_GridMaterialRed'"));
	if (TempMaterialRed.Succeeded())
	{
		GridMaterials.Add(EGridColor::RED, TempMaterialRed.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> TempMaterialBlue(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Saga/Materials/Grid/MI_GridMaterialBlue.MI_GridMaterialBlue'"));
	if (TempMaterialBlue.Succeeded())
	{
		GridMaterials.Add(EGridColor::BLUE, TempMaterialBlue.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> TempMaterialYello(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Saga/Materials/Grid/MI_GridMaterialYellow.MI_GridMaterialYellow'"));
	if (TempMaterialYello.Succeeded())
	{
		GridMaterials.Add(EGridColor::YELLO, TempMaterialYello.Object);
	}	
	ConstructorHelpers::FObjectFinder<UMaterialInterface> TempMaterialPurple(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Saga/Materials/Grid/MI_GridMaterialPurple.MI_GridMaterialPurple'"));
	if (TempMaterialPurple.Succeeded())
	{
		GridMaterials.Add(EGridColor::PURPLE, TempMaterialPurple.Object);
	}	
	ConstructorHelpers::FObjectFinder<UMaterialInterface> TempMaterialOrange(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Saga/Materials/Grid/MI_GridMaterialOrange.MI_GridMaterialOrange'"));
	if (TempMaterialOrange.Succeeded())
	{
		GridMaterials.Add(EGridColor::ORANGE, TempMaterialOrange.Object);
	}
	
}

void UGridViewComponent::InitViewComponent(AGridManager* gm, AGrid* ownergrid)
{
	
	OwnerGrid = ownergrid;
	//gm->GridOnOffDelegate.AddUObject(this, &UGridViewComponent::VisualOnOff);

}

void UGridViewComponent::BeginPlay()
{
	Super::BeginPlay();
	GridMeshComponent->SetupAttachment(GetOwner()->GetRootComponent());
	GridMeshComponent->SetRelativeLocation(GetOwner()->GetActorLocation());
	GridMeshComponent->AddRelativeLocation(FVector(0, 0, -0.1f));
	//VisualOff();
	//if (GetOwner())
	//{
	//	AGrid* ownerGrid = Cast<AGrid>(GetOwner());
	//	if (ownerGrid)
	//	{
	//		OwnerGrid = ownerGrid;
	//	}
	//}
	//if (OwnerGrid)
	//{
	// AGridManager* gm =	OwnerGrid->GetGridManager();
	// if (gm)
	// {
	//	 gm->GridOnOffDelegate.BindUFunction(this,"VisualOnOff");
	// }
	//}
}

void UGridViewComponent::ChangeMaterialColor_Implementation(EGridColor color)
{
	switch (color)
	{
	case EGridColor::WHITE:
		GridMeshComponent->SetMaterial(0, GridMaterials[EGridColor::WHITE]);

		break;
	case EGridColor::GREEN:
		GridMeshComponent->SetMaterial(0, GridMaterials[EGridColor::GREEN]);

		break;
	case EGridColor::RED:
		GridMeshComponent->SetMaterial(0, GridMaterials[EGridColor::RED]);

		break;
	case EGridColor::BLUE:
		GridMeshComponent->SetMaterial(0, GridMaterials[EGridColor::BLUE]);

		break;
	case EGridColor::YELLO:
		GridMeshComponent->SetMaterial(0, GridMaterials[EGridColor::YELLO]);

		break;
	case EGridColor::MAX:
		break;
	default:
		break;
	}

}

void UGridViewComponent::ChangeGridState(EGridState state)
{
	switch (state)
	{
	case EGridState::VALID:
		GridMeshComponent->SetMaterial(0, GridMaterials[EGridColor::GREEN]);
		break;
	case EGridState::INVALID:
		GridMeshComponent->SetMaterial(0, GridMaterials[EGridColor::RED]);
		break;
	case EGridState::FriendlyUnit:
		GridMeshComponent->SetMaterial(0, GridMaterials[EGridColor::BLUE]);
		break;
	case EGridState::EnemyUnit:
		GridMeshComponent->SetMaterial(0, GridMaterials[EGridColor::YELLO]);
		break;
	case EGridState::MAX:
		break;
	default:
		break;
	}

}

void UGridViewComponent::VisualOn()
{
	GridMeshComponent->SetVisibility(true);
}

void UGridViewComponent::VisualOff()
{
	GridMeshComponent->SetVisibility(false);
}

void UGridViewComponent::VisualOnOff(FIntPoint Index, bool OnOff)
{
	if (OwnerGrid)
	{
		if (Index == OwnerGrid->GetGridIndex_Implementation())
		{
			GridMeshComponent->SetVisibility(OnOff);
		}
	}

}



