// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/MyUserWidget.h"
#include "Data/Enum/Enums.h"
#include "CharacterProgressBar.generated.h"

/**
 * 
 */


UCLASS()
class SAGA_API UCharacterProgressBar : public UMyUserWidget
{
	GENERATED_BODY()
	
public:
	UCharacterProgressBar(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
public:
	void SetMaxValue(ECharValue Type,float NewMaxValue);
	void UpdateValueBar(ECharValue Type,float NewCurrentValeu);
protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpValueProgerssBar;
	TObjectPtr<class UProgressBar> MpValueProgerssBar;
	TObjectPtr<class UProgressBar> StaminaValueProgerssBar;

	UPROPERTY()
	float HpMaxValue;
	UPROPERTY()
	float MpMaxValue;
	UPROPERTY()
	float StaminaMaxValue;
	
};
