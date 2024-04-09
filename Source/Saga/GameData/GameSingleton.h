// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Grid/GridManager.h"
#include "GameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGameSingleton, Error, All);

/**
 * 
 */
UCLASS()
class SAGA_API UGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UGameSingleton();
	static UGameSingleton& Get();

public:


private:

};
