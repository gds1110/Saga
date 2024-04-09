// Fill out your copyright notice in the Description page of Project Settings.


#include "Preview/PreviewActor.h"

// Sets default values
APreviewActor::APreviewActor()
{

	PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMeshComponment"));
}

void APreviewActor::ChangeStaticMesh(UStaticMesh* Inmesh)
{
	PreviewMesh->SetStaticMesh(Inmesh);

}

bool APreviewActor::CheckMesh()
{
	if (PreviewMesh->GetStaticMesh())
	{
		return true;
	}
	else
	{
		return false;
	}
}





