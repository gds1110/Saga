// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PreviewActor.generated.h"

UCLASS()
class SAGA_API APreviewActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APreviewActor();


	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> PreviewMesh;

	UPROPERTY(EditAnywhere)
	TMap<bool, UMaterial*> Materials;


public:
	void ChangeStaticMesh(UStaticMesh* Inmesh);
	bool CheckMesh();
};
