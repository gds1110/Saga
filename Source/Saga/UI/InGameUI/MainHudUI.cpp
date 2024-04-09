// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameUI/MainHudUI.h"
#include "UI/InGameUI/TurnOrderListUI.h"
#include "UI/InGameUI/CharacterInformationUI.h"
#include "Character/CharacterBase.h"
#include "Grid/Grid.h"

UMainHudUI::UMainHudUI(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UMainHudUI::ChangeInfo(AGrid* TargetGrid)
{
	CharacterHudUI->SelectedCharacter = TargetGrid->GetOnCharacter();
	CharacterHudUI->ChangeUIInfo();

}

void UMainHudUI::ChangeInfo(ACharacterBase* TargetCharacter)
{
	CharacterHudUI->SelectedCharacter = TargetCharacter;
	CharacterHudUI->ChangeUIInfo();
}

void UMainHudUI::AddTurnOrder(AGrid* TargetGrid)
{
	//사용안함
	TurnOrderList->AddChildOrder(TargetGrid->GetOnCharacter());
}

void UMainHudUI::RemoveTurnOrder(AGrid* TargetGrid)
{

}

void UMainHudUI::RemoveTurnOrder(ACharacterBase* TargetCharacter)
{

}

void UMainHudUI::RefreshTurnOrder()
{

}



void UMainHudUI::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Log, TEXT("Screen NativeConstruct Appear"));

	
	FName TurnOrderListName = FName(TEXT("OrderListUI"));
	TurnOrderList = Cast<UTurnOrderListUI>(GetWidgetFromName(TurnOrderListName));
	if (TurnOrderList != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Find WBP_OrderList"));

	}
	FName CharacterHudUIName = FName(TEXT("CharInfoUI"));
	CharacterHudUI = Cast<UCharacterInformationUI>(GetWidgetFromName(CharacterHudUIName));
	if (CharacterHudUI != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Find WBP_CharacterInfoUI"));

	}
}
