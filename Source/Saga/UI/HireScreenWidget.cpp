// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HireScreenWidget.h"
#include "CharacterSlot.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"


void UHireScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//FName ListName = FName(TEXT("tTarvenList"));
	//tHireListWidget = GetWidgetFromName<UUserWidget>(ListName);
	//ensure(tHireListWidget);

	UDataTable* DT_CharData = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Saga/Data/Character/DT_CharStructTable.DT_CharStructTable'"));

	TArray<FName> RowNames = DT_CharData->GetRowNames();

	for (int i = 0; i < RowNames.Num(); ++i)
	{
		FCharacterStuructData charData = *(DT_CharData->FindRow<FCharacterStuructData>(RowNames[i], RowNames[i].ToString()));
		UE_LOG(LogTemp, Log, TEXT("char name : %s"), *charData.D_CharacterName);


	}
}
