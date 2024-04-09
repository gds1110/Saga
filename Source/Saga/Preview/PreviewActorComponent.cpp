// Fill out your copyright notice in the Description page of Project Settings.


#include "Preview/PreviewActorComponent.h"

// Sets default values for this component's properties
UPreviewActorComponent::UPreviewActorComponent()
{

	PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMeshComponment"));
}

void UPreviewActorComponent::ChangeStaticMesh(UStaticMesh*	Inmesh)
{
	PreviewMesh->SetStaticMesh(Inmesh);
}


