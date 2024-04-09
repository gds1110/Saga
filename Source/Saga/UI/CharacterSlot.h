// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MyUserWidget.h"
#include "Data/CharacterStuructData.h"
#include "CharacterSlot.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API UCharacterSlot : public UMyUserWidget
{
	GENERATED_BODY()
	
public:
	UCharacterSlot(const FObjectInitializer& ObjectInitializer);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FCharacterStuructData Data;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UButton> UCharButton;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess="true"))
	TObjectPtr<class UImage> UCharThumnail;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> UNameText;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> UStr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> UAgi;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> UInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UMyUserWidget> OwnerWidget;
protected:
	virtual void NativeConstruct() override;

public:
	void SetSlotInformation();
	UFUNCTION()
	void SetSloatData(FCharacterStuructData InputData);
	void SetOwnerWidget(UMyUserWidget* OwWidget);
	UFUNCTION(BlueprintCallable, Category = "InstanceFunction")
	void SetCurrentCharSlot();
};
