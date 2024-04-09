// Fill out your copyright notice in the Description page of Project Settings.


#include "Turn/TurnManager.h"
#include "UI/InGameUI/TurnOrderListUI.h"
#include "UI/InGameUI/CharacterOrderSlot.h"
#include "Character/CharacterBase.h"
#include "SagaGameMode.h"
#include "UI/InGameUI/MainHudUI.h"


UTurnManager::UTurnManager(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	bTickable = true;
	bTickableWhenPaused = false;
	UE_LOG(LogTemp,Log,TEXT("TurnManager I'm Born"));
}

void UTurnManager::StartNextTurn()
{
	CalculateTurnOrder();
	FTimerHandle tHandle;
	const float Delay = 2.0f;
	GetWorld()->GetTimerManager().SetTimer(tHandle, this, &UTurnManager::StartTurn, Delay, false);
}

void UTurnManager::StartTurn()
{
	//if (CharacterList[CurrentNumber] != nullptr)
	//{
	//	if (CharacterList[CurrentNumber]->IsMyTurnEnd == false)
	//	{
	//		CharacterList[CurrentNumber]->TurnOn();
	//	}
	//}
	if (UseTurnOrder.Num() > 0)
	{
		CurrentTurnCharacter = UseTurnOrder[0];
		UseTurnOrder.RemoveAt(0);
	}

	if(CurrentTurnCharacter != nullptr)
	{
		if (CurrentTurnCharacter->IsMyTurnEnd == false)
		{
			CurrentTurnCharacter->TurnOn();
			MyGameMode->CharacterHudUI->ChangeInfo(CurrentTurnCharacter);
		}
	}
}

void UTurnManager::CalculateTurnOrder()
{

	if (CharacterList.Num() > 0) {

		for (ACharacterBase* Char : CharacterList)
		{
			Char->RollDice();
		}
		CharacterList.Sort([](const ACharacterBase& char1, const ACharacterBase& char2) {
			return char1.CurrentAtkSpd > char2.CurrentAtkSpd;
			});
	}
	for (int i = 0; i < CharacterList.Num(); i++)
	{
		UseTurnOrder.Add(CharacterList[i]);
		//CharacterList[i]->MyTurnNumber = i;
		UseTurnOrder[i]->IsMyTurnEnd = false;
		UE_LOG(LogTemp, Log, TEXT("Char Name : %s, Char Spd : %f"),*CharacterList[i]->GetName(),CharacterList[i]->CurrentAtkSpd);
	}
	if (TurnOrderListUI != nullptr)
	{
		TurnOrderListUI->ClearOrderAndCharacters();
		TurnOrderListUI->Characters = CharacterList;
		TurnOrderListUI->RefreshOrder();
	}
	CurrentNumber = 0;
	MaxNumber = CharacterList.Num();
	//CurrentTurnCharacter = UseTurnOrder[0];
}

void UTurnManager::UpdateOrderUI()
{
	if (CharacterList.Num() > 0)
	{
		//TurnOrderListUI->
	}

}

void UTurnManager::RemoveAtTurnMgr(ACharacterBase* TargetCharacter)
{
	if (TurnOrderListUI != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Remove At TurnMgr"));
		
		CharacterList.Remove(TargetCharacter);
		TurnOrderListUI->RemoveChildOrder(TargetCharacter);
		MaxNumber = CharacterList.Num();
	}

}
void UTurnManager::RemoveAtOnlyUI(ACharacterBase* TargetCharacter)
{
	if (TurnOrderListUI != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Remove At Only TurnMgr UI"));

		TurnOrderListUI->RemoveChildOnlyUILIST(TargetCharacter);
	}

}
//전체적인 턴 체크
void UTurnManager::CheckTurnEnd()
{
	if (CharacterList.Num() > 0)
	{
	/*	if (CurrentNumber == CharacterList.Num() && CharacterList[CurrentNumber]->IsMyTurnEnd == true)
		{
			if (CharacterList.Num() > 0) {

				for (ACharacterBase* InChar : CharacterList)
				{
					InChar->IsMyTurn = false;
					InChar->IsMyTurnEnd = false;
					InChar->MyTurnNumber = -99;
				}

				StartNextTurn();
			}
		}*/
		bool CheckEnd = false;
		for (ACharacterBase* InChar : CharacterList)
		{
			if (InChar->IsMyTurnEnd == false)
			{
				CheckEnd = true;
			}
			if (CheckEnd == true)
			{
				UE_LOG(LogTemp, Log, TEXT("Not End"));
			}
		}
		if (CheckEnd == false)
		{
			StartNextTurn();
		}
		else
		{
			if (UseTurnOrder.Num()>0)
			{
				UE_LOG(LogTemp, Log, TEXT("Do Next Turn Char"));
				StartTurn();
			}
			else
			{
				StartNextTurn();
			}
		}
	}
}


void UTurnManager::CallTimer()
{
	FTimerHandle tHandle;
	const float Delay = 1.0f;
	GetWorld()->GetTimerManager().SetTimer(tHandle, this, &UTurnManager::TestTimer, Delay, false);
}

void UTurnManager::TestTimer()
{
	GEngine->AddOnScreenDebugMessage(1, 2, FColor::Red, "Hello World");

}

void UTurnManager::Tick(float DeltaTime)
{

	//UE_LOG(LogTemp, Log, TEXT("Turn MANAGER TICK TEST"));
	//CheckTurnEnd();
}

bool UTurnManager::IsTickable() const
{
	return bTickable;
}

bool UTurnManager::IsTickableInEditor() const
{
	return false;
}

bool UTurnManager::IsTickableWhenPaused() const
{
	return bTickableWhenPaused;
}

TStatId UTurnManager::GetStatId() const
{
	return TStatId();
}

UWorld* UTurnManager::GetWorld() const
{
	return GetOuter()->GetWorld();
}
