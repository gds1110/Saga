// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/InGameWidgetBase.h"
#include "MainHudUI.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API UMainHudUI : public UInGameWidgetBase
{
	GENERATED_BODY()
public:
	UMainHudUI(const FObjectInitializer& ObjectInitializer);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UIWidget)
	TObjectPtr<class UTurnOrderListUI> TurnOrderList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UIWidget)
	TObjectPtr<class UCharacterInformationUI> CharacterHudUI;

public:
	void ChangeInfo(class AGrid* TargetGrid);
	void ChangeInfo(class ACharacterBase* TargetCharacter);
	void AddTurnOrder(class AGrid* TargetGrid);
	void RemoveTurnOrder(class AGrid* TargetGrid);
	void RemoveTurnOrder(class ACharacterBase* TargetCharacter);
	void RefreshTurnOrder();
protected:
	virtual void NativeConstruct() override;
};
