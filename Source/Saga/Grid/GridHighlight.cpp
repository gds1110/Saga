// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GridHighlight.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include "SagaPlayerController.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
AGridHighlight::AGridHighlight()
{

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	HighlightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HighlightMesh"));
	HighlightMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempTileMesh(TEXT("/Script/Engine.StaticMesh'/Game/Saga/Meshs/Highlight/HighLightFlat.HighLightFlat'"));
	if (TempTileMesh.Succeeded())
	{
		HighlightMesh->SetStaticMesh(TempTileMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterialInterface> TempMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Saga/Materials/Highlight/MI_groundHighlight_Mat.MI_groundHighlight_Mat'"));
	if (TempMaterial.Succeeded())
	{
		HighlightMesh->SetMaterial(0, TempMaterial.Object);
	}

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HLState = HighlightState::DEFAULT;

	TArray<UPrimitiveComponent*> ColliderComponents;
	GetComponents<UPrimitiveComponent>(ColliderComponents);

	// 찾은 모든 콜라이더 비활성화
	for (UPrimitiveComponent* PrimitiveComponent : ColliderComponents)
	{
		// 콜라이더를 비활성화
		PrimitiveComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AGridHighlight::ChangeHighlightColor(HighlightState state)
{

	FLinearColor ChangedColor;

	switch (state)
	{
	case HighlightState::DEFAULT:
		ChangedColor = FLinearColor(0, 1, 0);
		break;
	case HighlightState::SELECTED:
		ChangedColor = FLinearColor(0, 0, 1);
		break;
	case HighlightState::INVAILD:
		ChangedColor = FLinearColor(1, 0, 0);
		break;
	case HighlightState::VAILD:
		ChangedColor = FLinearColor(1, 1, 0);
		break;
	case HighlightState::MAX:
		break;
	default:
		break;
	}
	
	ChangeMaterialColor(ChangedColor);
}

// Called when the game starts or when spawned
void AGridHighlight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridHighlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridHighlight::ChangeMaterialColor(FLinearColor NewColor)
{
	if (HighlightMesh)
	{
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(HighlightMesh->GetMaterial(0), this);
		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue("BaseColor", NewColor);
			HighlightMesh->SetMaterial(0, DynamicMaterial);
		}
	}

}

void AGridHighlight::ReturnSelf()
{
	//if()
	if (PoolController == nullptr) return;
	PoolController->ReturnOjbect(this);
	SetActive(false);
}

void AGridHighlight::SetActive(bool IsActive)
{
	bActive = IsActive;
	SetActorHiddenInGame(!bActive);
}

void AGridHighlight::ChangeColor_Implementation(FLinearColor NewColor)
{
}

