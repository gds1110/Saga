// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "CharacterHireScreen.h"

UCharacterSlot::UCharacterSlot(const FObjectInitializer& ObjectInitializer) 
	:Super(ObjectInitializer)
{
	
}

void UCharacterSlot::NativeConstruct()
{
	Super::NativeConstruct();

	UCharThumnail = Cast<UImage>(GetWidgetFromName(TEXT("CharThumnail")));
	ensure(UCharThumnail);
	UCharButton = Cast<UButton>(GetWidgetFromName(TEXT("WButton")));
	ensure(UCharButton);
	UNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameText")));
	ensure(UNameText);
	UStr = Cast<UTextBlock>(GetWidgetFromName(TEXT("StrText")));
	ensure(UStr);
	UAgi = Cast<UTextBlock>(GetWidgetFromName(TEXT("AgiText")));
	ensure(UAgi);
	UInt = Cast<UTextBlock>(GetWidgetFromName(TEXT("IntText")));
	ensure(UInt);

	if (UCharThumnail != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Find UCharThumnail"));
		UCharThumnail->SetBrushFromTexture(Data.D_Thumbnail);
	}	
	if (UCharButton != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Find UCharButton"));
		UCharButton->OnClicked.AddDynamic(this, &UCharacterSlot::SetCurrentCharSlot);
	}
	if (UNameText != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Find UNameText"));
		UNameText->SetText(FText::FromString(Data.D_CharacterName));

	}
	if (UStr != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Find UStr"));
		FString InString = FString::Printf(TEXT("Str : %d"), Data.D_Str);
		FText InText = FText::FromString(InString);
		UStr->SetText(InText);
	}
	if (UAgi != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Find UAgi"));
		FString InString = FString::Printf(TEXT("Agi : %d"), Data.D_Agi);
		FText InText = FText::FromString(InString);
		UAgi->SetText(InText);
	}
	if (UInt != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Find UInt"));
		FString InString = FString::Printf(TEXT("Int : %d"), Data.D_Int);
		FText InText = FText::FromString(InString);
		UInt->SetText(InText);
	}
	/*UNameText->SetText(FText::FromString(Data.D_CharacterName));
	UCharThumnail->SetBrushFromTexture(Data.D_Thumbnail);
	UStr->SetText(FText::AsNumber(Data.D_Str));
	UAgi->SetText(FText::AsNumber(Data.D_Agi));
	UInt->SetText(FText::AsNumber(Data.D_Int));*/

}

void UCharacterSlot::SetSlotInformation()
{



	/*
		UCharThumnail->SetBrushFromTexture(Data.D_Thumbnail);
		UNameText->SetText(FText::FromString(Data.D_CharacterName));
		UStr->SetText(FText::AsNumber(Data.D_Str));
		UAgi->SetText(FText::AsNumber(Data.D_Agi));
		UInt->SetText(FText::AsNumber(Data.D_Int));
	*/
}

void UCharacterSlot::SetSloatData(FCharacterStuructData InputData)
{
	Data = InputData;
	if (Data.D_CharacterName!="")
	{
		UE_LOG(LogTemp, Log, TEXT("Data Found"));
		UE_LOG(LogTemp, Log, TEXT("%s"),*Data.D_CharacterName);

	}
}

void UCharacterSlot::SetOwnerWidget(UMyUserWidget* OwWidget)
{
	OwnerWidget = OwWidget;
}

void UCharacterSlot::SetCurrentCharSlot()
{
	if (OwnerWidget)
	{
		Cast<UCharacterHireScreen>(OwnerWidget)->SetCurrentCharSlot(this);
	}
}
