// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GridActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGridActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SAGA_API IGridActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category = "MyInterface")
	FIntPoint GetGridIndex();
	UFUNCTION(BlueprintNativeEvent, Category = "MyInterface")
	FVector GetGridLocation();
};
