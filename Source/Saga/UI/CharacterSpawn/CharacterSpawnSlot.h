// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/InGameWidgetBase.h"
#include "Data/CharacterStuructData.h"
#include "UI/CharacterSpawn/CharacterSpawnButton.h"
#include "Components/UniformGridPanel.h"
#include "CharacterSpawnSlot.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API UCharacterSpawnSlot : public UInGameWidgetBase
{
	GENERATED_BODY()
	
public:
	UCharacterSpawnSlot(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUniformGridPanel* SpawnCharGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCharacterSpawnButton> SpawnBtnClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UCharacterSpawnButton>> CharacterButtons;

	UPROPERTY(EditAnywhere, Category = "CharList")
	TSubclassOf<class UCharacterSlot> CharSlot;

protected:
	virtual void NativeConstruct() override;

public:
	void CreateSpawnCharacterButton(FCharacterStuructData data);
};
