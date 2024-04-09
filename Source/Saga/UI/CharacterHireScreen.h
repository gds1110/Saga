// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MyUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Data/CharacterStuructData.h"
#include "CharacterHireScreen.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API UCharacterHireScreen : public UMyUserWidget
{
	GENERATED_BODY()
	
public:
	UCharacterHireScreen(const FObjectInitializer& ObjectInitializer);
	
	protected:			
	virtual void NativeConstruct() override;
	

protected:
	UPROPERTY(EditAnywhere,Category="CharList")
	class UWrapBox* tHireListWidget;
	UPROPERTY(EditAnywhere,Category="CharList")
	class UWrapBox* MyCharListWidget;
	UPROPERTY(EditAnywhere,Meta=(RowType=CharacterStuructData))
	FDataTableRowHandle CharRowHandle;
	UPROPERTY(EditAnywhere, Category = "CharList")
	TSubclassOf<class UCharacterSlot> CharSlot; 
	UPROPERTY(EditAnywhere,Category="CharList")
	TArray<TObjectPtr<class UCharacterSlot>> InTarvenList;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UButton> HireButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UDataTable* CharactersDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCharacterSlot> CurrentSlot;

public:

	UFUNCTION()
	void AddHireSlot(FCharacterStuructData CharStructData);
	UFUNCTION(BlueprintCallable, Category = "InstanceFunction")
	void AddInMyList();
	UFUNCTION(BlueprintCallable, Category = "InstanceFunction")
	void ShowMyCharacters();
	UFUNCTION(BlueprintCallable, Category = "InstanceFunction")
	void SetCurrentCharSlot(class UCharacterSlot* InputSlot);
};
