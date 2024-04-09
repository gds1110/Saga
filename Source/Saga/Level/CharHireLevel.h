// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/BaseLevel.h"
#include "CharHireLevel.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API ACharHireLevel : public ABaseLevel
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
};
