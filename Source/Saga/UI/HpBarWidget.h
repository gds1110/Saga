// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.h"
#include "HpBarWidget.generated.h"


/**
 * 
 */
UCLASS()
class SAGA_API UHpBarWidget : public UMyUserWidget
{
	GENERATED_BODY()
	
public:
	UHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateHpBar(float NewCurrentHp);
protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgerssBar;

	UPROPERTY()
	float MaxHp;
};
