// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/InGameWidgetBase.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Data/CharacterStuructData.h"
#include "CharacterOrderSlot.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API UCharacterOrderSlot : public UInGameWidgetBase, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	class UImage* UCharThumnail;
	UPROPERTY(BlueprintReadWrite)
	class UTexture2D* UCharImg;
	UPROPERTY(BlueprintReadWrite)
	class UBorder* UCharBorder;
	UPROPERTY()
	FCharacterStuructData uidata;
public: 
	UPROPERTY()
	class ACharacterBase* OwnerCharacter;
	UPROPERTY()
	class UTurnOrderListUI* OwnerList;
public:
	
	void ChangeThumnail(class UTexture2D* InImage);
	void ChangeThumnail(class UTexture2D* InImage,EPlayerType Type);
	void SetCharImage(class UTexture2D* InImage);
protected:
	virtual void NativeConstruct() override;

	//virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	//
	//virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
};
