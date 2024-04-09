// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Spell/SpellList.h"
#include "Character/CharacterBase.h"
#include "Components/Button.h"



void USpellList::AddSpell(AChracterBase* InCharacter)
{

}

void USpellList::ChangeSpell(ACharacterBase* InCharacter)
{
	if (InCharacter != nullptr)
	{
		CurrentCharacter = InCharacter;
		if (InCharacter->SpellDatas.IsEmpty() == false)
		{
			ButtonOne->ChangeOwnerCharacter(InCharacter);
			ButtonTwo->ChangeOwnerCharacter(InCharacter);
			ButtonThree->ChangeOwnerCharacter(InCharacter);
			ButtonOne->ChangeData(CurrentCharacter->SpellDatas[0]);
			ButtonTwo->ChangeData(CurrentCharacter->SpellDatas[1]);
			ButtonThree->ChangeData(CurrentCharacter->SpellDatas[2]);
		}
		MoveButton->OnClicked.AddDynamic(CurrentCharacter,&ACharacterBase::DeployGrids);
	}
}

void USpellList::ClearList()
{
	ButtonOne->ClearUI();
	ButtonTwo->ClearUI();
	ButtonThree->ClearUI();
	if (CurrentCharacter != nullptr)
	{
		if (MoveButton->OnClicked.IsBound())
		{
			MoveButton->OnClicked.RemoveDynamic(CurrentCharacter, &ACharacterBase::DeployGrids);
		}

		CurrentCharacter = nullptr;
	}
}

void USpellList::NativeConstruct()
{
	MyHorizontalBox = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("InHorizontalBox")));
	ButtonOne = Cast<USpellButton>(GetWidgetFromName(TEXT("Spell_One")));
	ButtonTwo = Cast<USpellButton>(GetWidgetFromName(TEXT("Spell_Two")));
	ButtonThree = Cast<USpellButton>(GetWidgetFromName(TEXT("Spell_Three")));
	MoveButton = Cast<UButton>(GetWidgetFromName(TEXT("MoveBtn")));
}
