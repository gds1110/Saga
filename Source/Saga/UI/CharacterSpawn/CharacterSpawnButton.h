// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/InGameWidgetBase.h"
#include "Data/CharacterStuructData.h"
#include "CharacterSpawnButton.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API UCharacterSpawnButton : public UInGameWidgetBase
{
	GENERATED_BODY()
	
public:
	UCharacterSpawnButton(const FObjectInitializer& ObjectInitializer);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FCharacterStuructData Data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UImage* UCharThumnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* UCharButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* UNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class ASagaPlayerController* PlayerController;
public:
	UFUNCTION(BlueprintCallable, Category = "InstanceFunction")
	void SetSpawnButton(FCharacterStuructData InputData);
	UFUNCTION(BlueprintCallable, Category = "InstanceFunction")
	void SetSpawnInfo();

protected:
	virtual void NativeConstruct() override;

};
