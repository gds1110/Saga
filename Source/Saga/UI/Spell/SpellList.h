// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MyUserWidget.h"
#include "SpellButton.h"
#include "Components/HorizontalBox.h"
#include "SpellList.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API USpellList : public UMyUserWidget
{
	GENERATED_BODY()
	
public:
	//USpellList(const FObjectInitializer& ObjectInitializer);

	TArray<USpellButton*> ChildButtons;
	UHorizontalBox* MyHorizontalBox;
	//FCharacterCombatData SpellData;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<class ACharacterBase> CurrentCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpellButton> ButtonOne;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpellButton> ButtonTwo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpellButton> ButtonThree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UButton> MoveButton;

	void AddSpell(class AChracterBase* InCharacter);
	void ChangeSpell(class ACharacterBase* InCharacter);
	void ClearList();
private:


protected:
	virtual void NativeConstruct() override;

};
