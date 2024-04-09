// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PreviewActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAGA_API UPreviewActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPreviewActorComponent();

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> PreviewMesh;

	UPROPERTY(EditAnywhere)
	TMap<bool,UMaterial*> Materials;



public:	
	void ChangeStaticMesh(UStaticMesh* Inmesh);
};
