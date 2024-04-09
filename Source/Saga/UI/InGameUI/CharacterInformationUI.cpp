// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameUI/CharacterInformationUI.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Character/CharacterBase.h"
#include "UI/Spell/SpellList.h"
#include "CharacterStat/CharacterStatComponent.h"
#include "UI/CharacterProgressBar.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"


UCharacterInformationUI::UCharacterInformationUI(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> DefaultImgRef(TEXT("/Script/Engine.Texture2D'/Game/Saga/Textures/Portrait.Portrait'"));
	if (DefaultImgRef.Succeeded())
	{
		DefaultImg = DefaultImgRef.Object;
	}
}

void UCharacterInformationUI::NativeConstruct()
{
	UCharThumnail = Cast<UImage>(GetWidgetFromName(TEXT("CharThumnail")));
	ensure(UCharThumnail);
	UNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameText")));
	ensure(UNameText);
	UStr = Cast<UTextBlock>(GetWidgetFromName(TEXT("StrText")));
	ensure(UStr);
	UAgi = Cast<UTextBlock>(GetWidgetFromName(TEXT("AgiText")));
	ensure(UAgi);
	UInt = Cast<UTextBlock>(GetWidgetFromName(TEXT("IntText")));
	ensure(UInt);
	UHp = Cast<UTextBlock>(GetWidgetFromName(TEXT("HpText")));
	UMana = Cast<UTextBlock>(GetWidgetFromName(TEXT("MpText")));
	USta = Cast<UTextBlock>(GetWidgetFromName(TEXT("StaText")));
	UMoveRange = Cast<UTextBlock>(GetWidgetFromName(TEXT("MoveText")));
	SpellListUI = Cast<USpellList>(GetWidgetFromName(TEXT("CharSpellList")));

	HpValueProgerssBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHp")));
	ensure(HpValueProgerssBar);
	MpValueProgerssBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbMp")));
	ensure(MpValueProgerssBar);
	StaminaValueProgerssBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbSta")));
	ensure(StaminaValueProgerssBar);

	//DefaultImg = Findobject

}

void UCharacterInformationUI::ChangeUIInfo()
{
	ClearUIInfo();
	if (SelectedCharacter != nullptr)
	{
		SelectedCharacter->GetCharacterStat()->ValueChangeDelegate.BindUFunction(this, FName("RefreshUIInfo"));
		UCharacterStatComponent* TempData = SelectedCharacter->GetCharacterStat();

		int32 CurHp = TempData->CurrentHp;
		int32 MaxHp = TempData->MaxHp;
		FString StrString = FString::Printf(TEXT("STR : %d"), TempData->Str);
		FString AgiString = FString::Printf(TEXT("AGI : %d"), TempData->Agi);
		FString IntString = FString::Printf(TEXT("INT : %d"), TempData->Int);
		FString HpString = FString::Printf(TEXT("HP : %d / %d"),  CurHp, MaxHp);
		FString ManaString = FString::Printf(TEXT("MP : %d / %d"), TempData->CurrentMp, TempData->MaxMp);
		FString StaString = FString::Printf(TEXT("SP : %d / %d"), TempData->CurrentStamina, TempData->MaxStamina);
		FString MoveRangeString = FString::Printf(TEXT("MoveRange : %d"), TempData->MaxStamina);


		UCharThumnail->SetBrushFromTexture(TempData->D_Thumbnail);
		UNameText->SetText(FText::FromString(TempData->D_CharacterName));
		UStr->SetText(FText::FromString(StrString));
		UAgi->SetText(FText::FromString(AgiString));
		UInt->SetText(FText::FromString(IntString));
		UHp->SetText(FText::FromString(HpString));
		UMana->SetText(FText::FromString(ManaString));
		USta->SetText(FText::FromString(StaString));
		UMoveRange->SetText(FText::FromString(MoveRangeString));

		SpellListUI->ChangeSpell(SelectedCharacter);

		if (HpValueProgerssBar)
		{
			float HpPer = float(TempData->CurrentHp) / float(TempData->MaxHp);
			HpValueProgerssBar->SetPercent(HpPer);
		}
		if (MpValueProgerssBar)
		{
			float MpPer = float(TempData->CurrentMp) / float(TempData->MaxMp);
			MpValueProgerssBar->SetPercent(MpPer);
		}
		if (StaminaValueProgerssBar)
		{
			float StaPer = float(TempData->CurrentStamina) / float(TempData->MaxStamina);
			StaminaValueProgerssBar->SetPercent(StaPer);
		}


	}

}



void UCharacterInformationUI::ClearUIInfo()
{
	if (SelectedCharacter != nullptr)
	{
		if (SelectedCharacter->GetCharacterStat()->ValueChangeDelegate.IsBound())
		{
			SelectedCharacter->GetCharacterStat()->ValueChangeDelegate.Unbind();
		}
	}

	UCharThumnail->SetBrushFromTexture(DefaultImg);
	UNameText->SetText(FText::FromString("Name"));
	UStr->SetText(FText::FromString("STR : "));
	UAgi->SetText(FText::FromString("AGI : "));
	UInt->SetText(FText::FromString("INT : "));
	UHp->SetText(FText::FromString("HP :"));
	UMana->SetText(FText::FromString("MP :"));
	USta->SetText(FText::FromString("SP :"));
	UMoveRange->SetText(FText::FromString("MoveRange : "));
	SpellListUI->ClearList();

	if (HpValueProgerssBar)
	{
		HpValueProgerssBar->SetPercent(1);
	}
	if (MpValueProgerssBar)
	{
		MpValueProgerssBar->SetPercent(1);
	}
	if (StaminaValueProgerssBar)
	{
		StaminaValueProgerssBar->SetPercent(1);
	}
}

void UCharacterInformationUI::RefreshUIInfo()
{
	if (SelectedCharacter != nullptr)
	{
		
			SelectedCharacter->GetCharacterStat()->ValueChangeDelegate.BindUFunction(this, FName("RefreshUIInfo"));
			UCharacterStatComponent* TempData = SelectedCharacter->GetCharacterStat();

			int32 CurHp = TempData->CurrentHp;
			int32 MaxHp = TempData->MaxHp;
			FString StrString = FString::Printf(TEXT("STR : %d"), TempData->Str);
			FString AgiString = FString::Printf(TEXT("AGI : %d"), TempData->Agi);
			FString IntString = FString::Printf(TEXT("INT : %d"), TempData->Int);
			FString HpString = FString::Printf(TEXT("HP : %d / %d"), CurHp, MaxHp);
			FString ManaString = FString::Printf(TEXT("MP : %d / %d"), TempData->CurrentMp, TempData->MaxMp);
			FString StaString = FString::Printf(TEXT("SP : %d / %d"), TempData->CurrentStamina, TempData->MaxStamina);
			FString MoveRangeString = FString::Printf(TEXT("MoveRange : %d"), TempData->MaxStamina);


			UCharThumnail->SetBrushFromTexture(TempData->D_Thumbnail);
			UNameText->SetText(FText::FromString(TempData->D_CharacterName));
			UStr->SetText(FText::FromString(StrString));
			UAgi->SetText(FText::FromString(AgiString));
			UInt->SetText(FText::FromString(IntString));
			UHp->SetText(FText::FromString(HpString));
			UMana->SetText(FText::FromString(ManaString));
			USta->SetText(FText::FromString(StaString));
			UMoveRange->SetText(FText::FromString(MoveRangeString));

			SpellListUI->ChangeSpell(SelectedCharacter);

			if (HpValueProgerssBar)
			{
				float HpPer = float(TempData->CurrentHp) / float(TempData->MaxHp);
				HpValueProgerssBar->SetPercent(HpPer);
			}
			if (MpValueProgerssBar)
			{
				float MpPer = float(TempData->CurrentMp) / float(TempData->MaxMp);
				MpValueProgerssBar->SetPercent(MpPer);
			}
			if (StaminaValueProgerssBar)
			{
				float StaPer = float(TempData->CurrentStamina) / float(TempData->MaxStamina);
				StaminaValueProgerssBar->SetPercent(StaPer);
			}


	}
	else
	{
		ClearUIInfo();
	}


}
