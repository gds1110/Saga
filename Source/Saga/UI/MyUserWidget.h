// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMyUserWidget(const FObjectInitializer& ObjectInitializer);
public:
	FORCEINLINE void SetOwningActor(AActor* NewOwner) { OwningActor = NewOwner; }


protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Actor")
	TObjectPtr<AActor> OwningActor;
};
