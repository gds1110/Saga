// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SagaGameMode.generated.h"

UCLASS(minimalapi)
class ASagaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASagaGameMode();
	virtual	void StartPlay() override;
	void BreakCharacterDataTable();
	class AGridManager* GetGridManager();
	void UpdateCharacterList();
	void RemoveAtOrder(class ACharacterBase* InChar);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECurrentGameMode CurrentGameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTurnManager> TurnManager;

	UPROPERTY(EditAnywhere,Category=Grid)
	TObjectPtr<class AGridManager> GridManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EffectManager)
	TObjectPtr<class AEffectManager> EffectManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UIWidgetClass)
	TSubclassOf<class UCharacterSpawnScreen> SpawnScreenUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UIWidgetClass)
	TSubclassOf<class UMainHudUI> CharacterHudUIClass;

	UPROPERTY(EditAnywhere, Category = UIWidget)
	TObjectPtr<class UCharacterSpawnScreen> SpawnScreenUI;
	UPROPERTY(EditAnywhere, Category = UIWidget)
	TObjectPtr<class UMainHudUI> CharacterHudUI;

	UPROPERTY(VisibleAnywhere, Category = CharacterList)
	TArray<TObjectPtr<class ACharacterBase>> AllCharacterList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDataTable* CharactersDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<struct FCharacterStuructData> CharDatas;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsGameStarted;
	UFUNCTION(BlueprintCallable, Category = "TurnFunction")
	void StartGame();
	UFUNCTION(BlueprintCallable, Category = "TurnFunction")
	void StartTurn();
	UFUNCTION(BlueprintCallable, Category = "TurnFunction")
	void EndGame();
	UFUNCTION(BlueprintCallable, Category = "TurnFunction")
	void TurnDice();
};



