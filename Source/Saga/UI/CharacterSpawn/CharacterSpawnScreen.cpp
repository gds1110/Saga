// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterSpawn/CharacterSpawnScreen.h"
#include "Blueprint/WidgetTree.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"


UCharacterSpawnScreen::UCharacterSpawnScreen(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

	UE_LOG(LogTemp, Log, TEXT("Screen Appear"));

	//static ConstructorHelpers::FObjectFinder<UDataTable> CharDataTable(TEXT("/Script/Engine.DataTable'/Game/Saga/Data/Character/DT_Characters.DT_Characters'"));
	//if (CharDataTable.Succeeded())
	//{
	//	UE_LOG(LogTemp, Log, TEXT("Succeed Table"));
	//	CharactersDataTable = CharDataTable.Object;
	//}
}

void UCharacterSpawnScreen::CreateSpawnButton()
{
	if (CharDatas.Num() > 0)
	{
		for (const FCharacterStuructData InData : CharDatas)
		{
			if (InData.D_Type == EPlayerType::PLAYER)
			{
				PlayerSlot->CreateSpawnCharacterButton(InData);
			}
			else
			{
				EnemySlot->CreateSpawnCharacterButton(InData);
			}
		}
	}
	

}

void UCharacterSpawnScreen::SettingCharacterDataTable(UDataTable* InDataTable)
{
	CharactersDataTable = InDataTable;

	if (CharactersDataTable != NULL)
	{
		TArray<FName> RowNames = CharactersDataTable->GetRowNames();
		FDataTableRowHandle RowHandle;

		for (const FName& RowName : RowNames)
		{
			FCharacterStuructData* chardata = CharactersDataTable->FindRow<FCharacterStuructData>(RowName, " ");
			if (chardata)
			{
				CharDatas.Add(*chardata);
			}
			UE_LOG(LogTemp, Log, TEXT("%d"), CharDatas.Num());

		}


	}
	CreateSpawnButton();
}

void UCharacterSpawnScreen::SettingCharacterData(TArray<FCharacterStuructData> InCharDatas)
{
	CharDatas = InCharDatas;
	CreateSpawnButton();

}

void UCharacterSpawnScreen::NativeConstruct()
{
	Super::NativeConstruct();
	
	UE_LOG(LogTemp, Log, TEXT("Screen NativeConstruct Appear"));

	TArray<UWidget*> Children;
	WidgetTree->GetAllWidgets(Children);


	FName PlayerListName = FName(TEXT("PlayerSpawn"));
	PlayerSlot = Cast<UCharacterSpawnSlot>(GetWidgetFromName("PlayerSpawn"));
	if (PlayerSlot != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Find PlayerSpawn"));

	}
	
	FName EnemyListName = FName(TEXT("EnemySpawn"));
	EnemySlot = Cast<UCharacterSpawnSlot>(GetWidgetFromName("EnemySpawn"));
	if (EnemySlot != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Find EnemySpawn"));
	}


	//캐릭터 데이터 테이블 셋팅
	/*if (CharactersDataTable != NULL)
	{
		TArray<FName> RowNames = CharactersDataTable->GetRowNames();
		FDataTableRowHandle RowHandle;

		for (const FName& RowName : RowNames)
		{
			FCharacterStuructData* chardata = CharactersDataTable->FindRow<FCharacterStuructData>(RowName," ");
			if (chardata)
			{
				CharDatas.Add(*chardata);
			}
			UE_LOG(LogTemp, Log, TEXT("%d"),CharDatas.Num());

		}


	}
	CreateSpawnButton();*/
	//this->AddToViewport();
}
