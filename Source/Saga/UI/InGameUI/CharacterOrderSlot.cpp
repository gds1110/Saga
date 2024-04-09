// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameUI/CharacterOrderSlot.h"
#include "Components/Image.h"
#include "Character/CharacterBase.h"
#include "SagaPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Data/Enum/Enums.h"
#include "Components/Border.h"


void UCharacterOrderSlot::ChangeThumnail(UTexture2D* InImage)
{
	if (UCharThumnail == nullptr)
	{
		UCharThumnail = Cast<UImage>(GetWidgetFromName(TEXT("CharacterImg")));

	}
	if (UCharBorder == nullptr)
	{
		UCharBorder = Cast<UBorder>(GetWidgetFromName(TEXT("CharacterBorder")));
	}
	if (UCharThumnail != nullptr)
	{
		UCharThumnail->SetBrushFromTexture(InImage);
		SetCharImage(InImage);
	}
	if (UCharBorder != nullptr)
	{
		ASagaPlayerController* PController = Cast<ASagaPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PController->CurrentData.D_Type == EPlayerType::PLAYER)
		{
			UCharBorder->SetBrushColor(FLinearColor::Green);
		}
		else
		{
			UCharBorder->SetBrushColor(FLinearColor::Red);
		}
	}
}

void UCharacterOrderSlot::ChangeThumnail(UTexture2D* InImage, EPlayerType Type)
{
	if (UCharThumnail == nullptr)
	{
		UCharThumnail = Cast<UImage>(GetWidgetFromName(TEXT("CharacterImg")));

	}
	if (UCharBorder == nullptr)
	{
		UCharBorder = Cast<UBorder>(GetWidgetFromName(TEXT("CharacterBorder")));
	}
	if (UCharThumnail != nullptr)
	{
		UCharThumnail->SetBrushFromTexture(InImage);
		SetCharImage(InImage);
	}
	if (UCharBorder != nullptr)
	{
		if (Type == EPlayerType::PLAYER)
		{
			UCharBorder->SetBrushColor(FLinearColor::Green);
		}
		else
		{
			UCharBorder->SetBrushColor(FLinearColor::Red);
		}
	}

}

void UCharacterOrderSlot::SetCharImage(UTexture2D* InImage)
{
	UCharImg = InImage;
}

void UCharacterOrderSlot::NativeConstruct()
{
	Super::NativeConstruct();


	UCharThumnail = Cast<UImage>(GetWidgetFromName(TEXT("CharacterImg")));
	ensure(UCharThumnail);
	UCharBorder = Cast<UBorder>(GetWidgetFromName(TEXT("CharacterBorder")));

}

//
//void UCharacterOrderSlot::NativeOnListItemObjectSet(UObject* ListItemObject)
//{
//
//	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
//	if (UCharThumnail == nullptr)
//	{
//		UCharThumnail = Cast<UImage>(GetWidgetFromName(TEXT("CharacterImg")));
//		if (UCharImg != nullptr)
//		{
//			UCharThumnail->SetBrushFromTexture(UCharImg);
//
//		}
//	}
//	if (UCharBorder == nullptr)
//	{
//		UCharBorder = Cast<UBorder>(GetWidgetFromName(TEXT("CharacterBorder")));
//	}
//	const ACharacterBase* InCharacter = Cast<ACharacterBase>(ListItemObject);
//
//	//UCharThumnail->SetBrushFromTexture(UCharImg);
//	//ASagaPlayerController* PController = Cast<ASagaPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
//	//if (PController->CurrentData.D_Thumbnail != nullptr)
//	//{
//	//	UCharThumnail->SetBrushFromTexture(PController->CurrentData.D_Thumbnail);
//	//}
//	//if (PController->CurrentData.D_Type == EPlayerType::PLAYER)
//	//{
//	//	UCharBorder->SetBrushColor(FLinearColor::Green);
//	//}
//	//else
//	//{
//	//	UCharBorder->SetBrushColor(FLinearColor::Red);
//	//}
//	if (uidata.D_Thumbnail != nullptr)
//	{
//		UCharThumnail->SetBrushFromTexture(uidata.D_Thumbnail);
//	}
//
//	FVector2D WidgetPosition = UCharThumnail->GetCachedGeometry().GetAbsolutePosition();
//	UE_LOG(LogTemp, Log, TEXT("%f, %f"),WidgetPosition.X,WidgetPosition.Y);
//}
//
//void UCharacterOrderSlot::NativeOnItemSelectionChanged(bool bIsSelected)
//{
//}
