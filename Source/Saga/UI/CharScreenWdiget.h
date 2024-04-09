// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MyUserWidget.h"
#include "Data/CharacterStuructData.h"
#include "CharScreenWdiget.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API UCharScreenWdiget : public UMyUserWidget
{
	GENERATED_BODY()
	
	FCharacterStuructData data;

public:




protected:
	virtual void NativeConstruct() override;
	
};
