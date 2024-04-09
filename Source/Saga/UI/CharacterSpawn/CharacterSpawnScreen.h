// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/InGameWidgetBase.h"
#include "UI/CharacterSpawn/CharacterSpawnSlot.h"
#include "CharacterSpawnScreen.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API UCharacterSpawnScreen : public UInGameWidgetBase
{
	GENERATED_BODY()
	


public:
	UCharacterSpawnScreen(const FObjectInitializer& ObjectInitializer);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCharacterSpawnSlot> PlayerSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCharacterSpawnSlot> EnemySlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDataTable* CharactersDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCharacterStuructData> CharDatas;

public:
	void CreateSpawnButton();
	void SettingCharacterDataTable(class UDataTable* InDataTable);
	void SettingCharacterData(TArray<FCharacterStuructData> InCharDatas);

protected:
	virtual void NativeConstruct();
};
