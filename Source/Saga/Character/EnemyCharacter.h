// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API AEnemyCharacter : public ACharacterBase
{
	GENERATED_BODY()
public:

	AEnemyCharacter(const FObjectInitializer& ObjectInitializer);
};
