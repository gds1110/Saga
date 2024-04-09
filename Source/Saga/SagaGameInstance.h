// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Data/CharacterStuructData.h"
#include "SagaGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API USagaGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	TArray<FCharacterStuructData> MyCharacters;
};
