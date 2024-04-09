// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MyUserWidget.h"
#include "Data/CharacterCombatData.h" 
#include "SpellButton.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API USpellButton : public UMyUserWidget
{
	GENERATED_BODY()
	
public:
	USpellButton(const FObjectInitializer& ObjectInitializer);
	
public:
	UPROPERTY()
	FCharacterCombatData CurrentSpellData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UButton> SpellButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTexture2D> DefaultIcon;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ACharacterBase> OwnerCharacter;

public:
	void ChangeData(FCharacterCombatData InData);
	void ChangeOwnerCharacter(class ACharacterBase* InCharacter);
	void ClearUI();
	UFUNCTION()
	void ButtonAction();
protected:
	virtual void NativeConstruct() override;
};
