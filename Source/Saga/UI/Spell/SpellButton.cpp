// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Spell/SpellButton.h"
#include "Components/Button.h" 
#include "Character/CharacterBase.h"

USpellButton::USpellButton(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> DefaultImgRef(TEXT("/Script/Engine.Texture2D'/Game/Saga/Textures/SpellIcon/ICON_Question.ICON_Question'"));
	if (DefaultImgRef.Succeeded())
	{
		DefaultIcon = DefaultImgRef.Object;
	}
}

void USpellButton::ChangeData(FCharacterCombatData InData)
{
	CurrentSpellData = InData;

	if (InData.D_Thumbnail != nullptr)
	{
		SpellButton->WidgetStyle.Normal.SetResourceObject(InData.D_Thumbnail);
		SpellButton->WidgetStyle.Hovered.SetResourceObject(InData.D_Thumbnail);
		SpellButton->WidgetStyle.Pressed.SetResourceObject(InData.D_Thumbnail);
	}
	else
	{
		SpellButton->WidgetStyle.Normal.SetResourceObject(DefaultIcon);
		SpellButton->WidgetStyle.Hovered.SetResourceObject(DefaultIcon);
		SpellButton->WidgetStyle.Pressed.SetResourceObject(DefaultIcon);
	}
	
}

void USpellButton::ChangeOwnerCharacter(ACharacterBase* InCharacter)
{

	if (InCharacter == nullptr)
	{
		//OwnerCharacter = nullptr;
		ClearUI();
		return;
	}
	OwnerCharacter = InCharacter;

}

void USpellButton::ClearUI()
{
	CurrentSpellData = FCharacterCombatData();

	if (DefaultIcon != nullptr && SpellButton != nullptr)
	{
		SpellButton->WidgetStyle.Normal.SetResourceObject(DefaultIcon);
		SpellButton->WidgetStyle.Hovered.SetResourceObject(DefaultIcon);
		SpellButton->WidgetStyle.Pressed.SetResourceObject(DefaultIcon);
	}
	OwnerCharacter = nullptr;
}

void USpellButton::ButtonAction()
{
	if (OwnerCharacter != nullptr)
	{
		OwnerCharacter->ChangeCurrentSpell(CurrentSpellData);
		OwnerCharacter->SpellGridDeploy();
		OwnerCharacter->CurState = ECharacterState::SPELLREADY;
	}
}

void USpellButton::NativeConstruct()
{

	SpellButton = Cast<UButton>(GetWidgetFromName(TEXT("SpellBtn")));
	SpellButton->OnClicked.AddDynamic(this, &USpellButton::ButtonAction);
	ClearUI();
}
