// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterHireScreen.h"
#include "CharacterSlot.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "Components/WrapBox.h"
#include "Components/Button.h"
#include "SagaGameInstance.h"

UCharacterHireScreen::UCharacterHireScreen(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{

	static ConstructorHelpers::FObjectFinder<UDataTable> CharDataTable(TEXT("/Script/Engine.DataTable'/Game/Saga/Data/Character/Characters.Characters'"));
	if (CharDataTable.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("Succeed Table"));
		CharactersDataTable = CharDataTable.Object;
	}

	static ConstructorHelpers::FClassFinder<UCharacterSlot> CharslotClass(TEXT("/Game/Saga/UI/Character/WBP_CharSloatB.WBP_CharSloatB_C"));
	if (CharslotClass.Succeeded())
	{
		CharSlot = CharslotClass.Class;
		UE_LOG(LogTemp, Log, TEXT("Char Class FInd"));

	}


}

void UCharacterHireScreen::NativeConstruct()
{
	Super::NativeConstruct();

	

	FName ListName = FName(TEXT("TarvenCharBox"));
	tHireListWidget=Cast<UWrapBox>(GetWidgetFromName(ListName));
	if (tHireListWidget != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Find HireListWidget"));

	}

	FName CharListName = FName(TEXT("WMyListBox"));
	MyCharListWidget = Cast<UWrapBox>(GetWidgetFromName(CharListName));
	if (tHireListWidget != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Find WMyListBox"));

	}
	FName ButtonName = FName(TEXT("WHireButton"));
	HireButton = Cast<UButton>(GetWidgetFromName(ButtonName));
	if (HireButton != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Find HireButton"));
		HireButton->OnClicked.AddDynamic(this,&UCharacterHireScreen::AddInMyList);
	}
	
	TArray<FName> RowNames = CharactersDataTable->GetRowNames();
	for (int i = 0; i < RowNames.Num(); ++i)
	{
		FCharacterStuructData* charData = CharactersDataTable->FindRow<FCharacterStuructData>(RowNames[i],FString(""));
		AddHireSlot(*charData);
	}

}



void UCharacterHireScreen::AddHireSlot(FCharacterStuructData CharStructData)
{
	
	UCharacterSlot* newCharSlot = Cast<UCharacterSlot>(CreateWidget(GetWorld(), CharSlot));
	newCharSlot->SetSloatData(CharStructData);
	newCharSlot->SetOwnerWidget(this);
	Cast<UWrapBox>(tHireListWidget)->AddChild(newCharSlot);

	//newCharSlot->AddToViewport();
	InTarvenList.Add(newCharSlot);
			


}

void UCharacterHireScreen::AddInMyList()
{
	if (CurrentSlot)
	{
	/*	TODO:: USAGAGAMEINSTANCE 구현? 언리얼엔진에서 추가
		USagaGameInstance* GameInstance = Cast<USagaGameInstance>(GetGameInstance());
		GameInstance->MyCharacters.Add(CurrentSlot->Data);*/
		MyCharListWidget->AddChildToWrapBox(CurrentSlot);
		InTarvenList.Remove(CurrentSlot);
		Cast<UWrapBox>(tHireListWidget)->RemoveChild(CurrentSlot);
	}
}

void UCharacterHireScreen::ShowMyCharacters()
{
	if (MyCharListWidget)
	{
		MyCharListWidget->ClearChildren();
		USagaGameInstance* GameInstance = Cast<USagaGameInstance>(GetGameInstance());
		TArray<FCharacterStuructData> MyCharDatas = GameInstance->MyCharacters;
		if (MyCharDatas.Num() > 0)
		{
			for (int i = 0; i < MyCharDatas.Num(); ++i)
			{
				UCharacterSlot* newCharSlot = Cast<UCharacterSlot>(CreateWidget(GetWorld(), CharSlot));
				newCharSlot->SetSloatData(MyCharDatas[i]);
				MyCharListWidget->AddChildToWrapBox(newCharSlot);
			}
		}
	}
}

void UCharacterHireScreen::SetCurrentCharSlot(UCharacterSlot* InputSlot)
{
	CurrentSlot = InputSlot;
}

