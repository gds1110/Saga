// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/InGameWidgetBase.h"
#include "TurnOrderListUI.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API UTurnOrderListUI : public UInGameWidgetBase
{
	GENERATED_BODY()
public:
	UTurnOrderListUI(const FObjectInitializer& ObjectInitializer);
	
public:
	class UScrollBox* OrderList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UCharacterOrderSlot> OrderSlotClass;

	TArray<class ACharacterBase*> Characters;
public:
	void AddChildOrder(class ACharacterBase* InCharacter);
	void RemoveChildOrder(class ACharacterBase* OutCharacter);
	void RemoveChildOnlyUILIST(class ACharacterBase* OutCharacter);
	void RefreshOrder();
	void UpdateOrder(); //Characters에만 정렬
	void SortOrder();
	void ClearOrderAndCharacters();
protected:
	virtual void NativeConstruct() override;
};
