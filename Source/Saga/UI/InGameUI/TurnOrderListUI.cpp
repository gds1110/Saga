// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameUI/TurnOrderListUI.h"
#include "Components/StackBox.h"
#include "Character/CharacterBase.h"
#include "CharacterStat/CharacterStatComponent.h"
#include "Data/CharacterStuructData.h"
#include "UI/InGameUI/CharacterOrderSlot.h"
#include "Components/ScrollBox.h"
#include "Algo/Sort.h"

UTurnOrderListUI::UTurnOrderListUI(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UCharacterOrderSlot> TempOrderSlotClass(TEXT("/Game/Saga/UI/Character/WBP_OrderSlot.WBP_OrderSlot_C"));
	if (TempOrderSlotClass.Succeeded())
	{
		OrderSlotClass = TempOrderSlotClass.Class;
	}

}

void UTurnOrderListUI::AddChildOrder(ACharacterBase* InCharacter)
{
	if (InCharacter != nullptr)
	{
		
		UCharacterOrderSlot* tempSlot = CreateWidget<UCharacterOrderSlot>(GetWorld(), OrderSlotClass);
		Characters.Add(InCharacter);
		if (InCharacter->D_Thumbnail != nullptr)
		{
			UTexture2D* InImg = InCharacter->D_Thumbnail;
			tempSlot->ChangeThumnail(InImg, InCharacter->PlayerType);
		}
		tempSlot->OwnerCharacter = InCharacter; // delegate로 처리
		InCharacter->TurnOrderUI = tempSlot;
		OrderList->AddChild(tempSlot);

		//if (InCharacter->GetCharacterStat() != nullptr)
		//{
		//	UCharacterStatComponent* StatComp = InCharacter->GetCharacterStat();
		//	if (StatComp->GetCharacterStat().D_Thumbnail != nullptr)
		//	{
		//		UTexture2D* InImg = StatComp->D_Thumbnail;
		//		UCharacterOrderSlot* tempSlot = CreateWidget<UCharacterOrderSlot>(GetWorld(), OrderSlotClass);
		//		Characters.Add(InCharacter);
		//		tempSlot->ChangeThumnail(InImg,InCharacter->PlayerType);
		//		tempSlot->OwnerCharacter = InCharacter; // delegate로 처리
		//		tempSlot->uidata = StatComp->GetCharacterStat();
		//		InCharacter->TurnOrderUI = tempSlot;
		//		OrderList->AddChild(tempSlot);
		//	}
		//}
		//OrderList->AddChildToStackBox(tempSlot);
	}
}

void UTurnOrderListUI::RemoveChildOrder(ACharacterBase* OutCharacter)
{
	
	//OrderList->GetAllChildren();
	//OrderList->RemoveChild(OutCharacter->TurnOrderUI);
	if (Characters.Find(OutCharacter))
	{
		Characters.Remove(OutCharacter);
		OrderList->RemoveChild(OutCharacter->TurnOrderUI);
		//TurnManager로 이행
	}



	//RefreshOrder();
	//for (UCharacterOrderSlot* Slot :)
	//{
	//	// 액터의 값을 비교하여 특정 값과 일치하는지 확인
	//	if (Actor && Actor->GetValue() == SpecificValue)  // GetValue()는 실제 사용 중인 메서드나 변수에 따라 수정해야 합니다.
	//	{
	//		// 특정 값을 가진 액터를 찾았으면 해당 액터를 반환
	//		return Actor;
	//	}
	//}
}

void UTurnOrderListUI::RemoveChildOnlyUILIST(ACharacterBase* OutCharacter)
{
	//if (Characters.Find(OutCharacter))
	//{
	//	OrderList->RemoveChild(OutCharacter->TurnOrderUI);
	//	//TurnManager로 이행
	//}
	//RefreshOrder();

	if (OrderList->GetChildrenCount() > 0)
	{
		OrderList->ClearChildren();
		UE_LOG(LogTemp, Log, TEXT("Clear Children"));
	}
	/*for (ACharacterBase* InCharacter : Characters)
	{
		if (InCharacter == OutCharacter)
		{
			continue;
		}
		AddChildOrder(InCharacter);
	}*/
}

void UTurnOrderListUI::RefreshOrder()
{
	if (OrderList->GetChildrenCount() > 0)
	{
		OrderList->ClearChildren();
		UE_LOG(LogTemp, Log, TEXT("Clear Children"));
	}
	for (ACharacterBase* InCharacter : Characters)
	{
		AddChildOrder(InCharacter);
	}


}

void UTurnOrderListUI::UpdateOrder()
{
	
}

void UTurnOrderListUI::SortOrder()
{
	Characters.Sort([](const ACharacterBase& A, const ACharacterBase& B) {
		return A.CurrentAtkSpd > B.CurrentAtkSpd;
		});

	RefreshOrder();
}

void UTurnOrderListUI::ClearOrderAndCharacters()
{
	if (Characters.Num() > 0)
	{
		Characters.Empty();
	}

	if (OrderList->GetChildrenCount() > 0)
	{
		OrderList->ClearChildren();
		UE_LOG(LogTemp, Log, TEXT("Clear Children"));
	}
}


void UTurnOrderListUI::NativeConstruct()
{
	Super::NativeConstruct();


	FName OrderListName = FName(TEXT("OrderBox"));
	OrderList = Cast<UScrollBox>(GetWidgetFromName(OrderListName));
	if (OrderList != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Find OrderList"));

	}
}
