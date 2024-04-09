// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HireScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API UHireScreenWidget : public UUserWidget
{
	GENERATED_BODY()
public:
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere, Category = "CharList")
	TObjectPtr<UUserWidget> tHireListWidget;
};
