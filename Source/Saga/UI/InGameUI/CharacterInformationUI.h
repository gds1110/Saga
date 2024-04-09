// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CharacterSlot.h"
//#include "UI/Spell/SpellList.h"
#include "CharacterInformationUI.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API UCharacterInformationUI : public UCharacterSlot
{
	GENERATED_BODY()
	
public:
	UCharacterInformationUI(const FObjectInitializer& ObjectInitializer);
	UPROPERTY()
	TObjectPtr<class ACharacterBase> SelectedCharacter;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> UAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> UAttackRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> UHp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> UMana;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> USta;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> UMoveRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTexture2D> DefaultImg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpellList> SpellListUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProgressBar> HpValueProgerssBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProgressBar> MpValueProgerssBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProgressBar> StaminaValueProgerssBar;
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void ChangeUIInfo();

	UFUNCTION()
	void ClearUIInfo();
	UFUNCTION()
	void RefreshUIInfo();
	
};
