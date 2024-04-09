// Copyright Epic Games, Inc. All Rights Reserved.

#include "SagaGameMode.h"
#include "SagaPlayerController.h"
#include "SagaCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Grid/GridManager.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CharacterSpawn/CharacterSpawnScreen.h"
#include "UI/InGameUI/CharacterInformationUI.h"
#include "UI/InGameUI/MainHudUI.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Character/CharacterBase.h"
#include "Turn/TurnManager.h"
#include "Effect/EffectManager.h"



ASagaGameMode::ASagaGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ASagaPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	//if (PlayerPawnBPClass.Class != nullptr)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}


	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/Saga/Blueprints/CameraPawn/BP_CameraPawn.BP_CameraPawn_C'"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Saga/Blueprints/Controller/BP_SagaPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	//GridManager = CreateDefaultSubobject<AGridManager>("GridManager");
	//PlayerControllerClass = ASagaPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<UCharacterSpawnScreen> ScreenUIClass(TEXT("/Game/Saga/UI/SpawnUI/WBP_SpawnScreen.WBP_SpawnScreen_C"));
	if (ScreenUIClass.Succeeded())
	{
		SpawnScreenUIClass = ScreenUIClass.Class;
	}	
	static ConstructorHelpers::FClassFinder<UMainHudUI> HudUIClass(TEXT("/Game/Saga/UI/WBP_Game.WBP_Game_C"));
	if (HudUIClass.Succeeded())
	{
		CharacterHudUIClass = HudUIClass.Class;
	}
	if (SpawnScreenUIClass)
	{
		SpawnScreenUI = CreateWidget<UCharacterSpawnScreen>(GetWorld(), SpawnScreenUIClass);
		//SpawnScreenUI->AddToViewport();
		//SpawnScreenUI->SetVisibility(ESlateVisibility::Hidden);
	}
	if (CharacterHudUIClass)
	{
		CharacterHudUI = CreateWidget<UMainHudUI>(GetWorld(), CharacterHudUIClass);
		//CharacterHudUI->AddToViewport();
		//CharacterHudUI->SetVisibility(ESlateVisibility::Visible);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> CharDataTable(TEXT("/Script/Engine.DataTable'/Game/Saga/Data/Character/DT_Characters.DT_Characters'"));
	if (CharDataTable.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("Succeed Table"));
		CharactersDataTable = CharDataTable.Object;
	}
}

void ASagaGameMode::StartPlay()
{
	Super::StartPlay();
	FTransform SpawnTransform;
	//GridManager = GetWorld()->SpawnActorDeferred<AGridManager>(AGridManager::StaticClass(), SpawnTransform);
	//GridManager->GeneratedGrid(30, 35);
	//GridManager->FinishSpawning(SpawnTransform);


	
	if (SpawnScreenUIClass)
	{
		//SpawnScreenUI = CreateWidget<UCharacterSpawnScreen>(GetWorld(), SpawnScreenUIClass);
		SpawnScreenUI->AddToViewport();
		SpawnScreenUI->SetVisibility(ESlateVisibility::Visible);
		BreakCharacterDataTable();
		SpawnScreenUI->SettingCharacterData(CharDatas);
	}
	if (CharacterHudUIClass)
	{
		//CharacterHudUI = CreateWidget<UMainHudUI>(GetWorld(), CharacterHudUIClass);
		CharacterHudUI->AddToViewport();
		CharacterHudUI->SetVisibility(ESlateVisibility::Hidden);
	}

	TObjectPtr<AGridManager> GridMG = GetWorld()->SpawnActor<AGridManager>(AGridManager::StaticClass());
	GridManager = GridMG;
	GridManager->GeneratedGrid(17, 17);
	TurnManager = NewObject<UTurnManager>(this,UTurnManager::StaticClass(),TEXT("TurnManagerObject"));
	if (TurnManager != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), *TurnManager->GetName());
		TurnManager->TurnOrderListUI = CharacterHudUI->TurnOrderList;
		TurnManager->MyGameMode = this;
	}
	//EffectManager = NewObject<AEffectManager>(this, AEffectManager::StaticClass(), TEXT("EffectManagerObject"));
	//TurnManager = GetWorld()->SpawnActor<UTurnManager>(UTurnManager::StaticClass());
	TObjectPtr<AEffectManager> EffectMG = GetWorld()->SpawnActor<AEffectManager>(AEffectManager::StaticClass());
	EffectManager = EffectMG;
	EffectManager->ExpandPool();

	ASagaPlayerController* CurPC = Cast<ASagaPlayerController>(GetWorld()->GetFirstPlayerController());
	if (CurPC)
	{
		CurPC->EffectManager = EffectManager;
	}
	CurrentGameMode = ECurrentGameMode::AIMODE;
	IsGameStarted = false;
}

void ASagaGameMode::BreakCharacterDataTable()
{
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
}

AGridManager* ASagaGameMode::GetGridManager()
{
	if (GridManager != nullptr)
	{
		return GridManager;
	}
	
	return nullptr;

}

void ASagaGameMode::UpdateCharacterList()
{
	AllCharacterList.Empty();
	TurnManager->CharacterList.Empty();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterBase::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		ACharacterBase* MyCharacter = Cast<ACharacterBase>(Actor);
		if (MyCharacter)
		{
			AllCharacterList.Add(MyCharacter);
			TurnManager->CharacterList.Add(MyCharacter);

		}
	}
}

void ASagaGameMode::RemoveAtOrder(ACharacterBase* InChar)
{
	//TurnManager

}

void ASagaGameMode::StartGame()
{

	auto PlayControl = Cast<ASagaPlayerController>(GetWorld()->GetFirstPlayerController());
	//PlayControl->ControlMode
	if (PlayControl != nullptr)
	{
		PlayControl->ChangeUIMode();
		PlayControl->ControlMode = EControllerMode::Default;
		IsGameStarted = true;
		switch (CurrentGameMode)
		{
		case ECurrentGameMode::AIMODE:
			for (ACharacterBase* InChar : AllCharacterList)
			{
				InChar->IsAIMode = true;
			}
			TurnDice();
			StartTurn();
			break;
		case ECurrentGameMode::PLAYINGMODE:
			for (ACharacterBase* InChar : AllCharacterList)
			{
				if (InChar->PlayerType == EPlayerType::ENEMY)
				{
					InChar->IsAIMode = true;
				}
			}
			TurnDice();
			StartTurn();
			break;
		case ECurrentGameMode::SIMULMODE:
			break;
		case ECurrentGameMode::MAX:
			break;
		default:
			break;
		}
	}
}

void ASagaGameMode::StartTurn()
{
	TurnManager->StartTurn();
}

void ASagaGameMode::EndGame()
{


}

void ASagaGameMode::TurnDice()
{
	if (TurnManager != nullptr)
	{
		TurnManager->CalculateTurnOrder();
	}
}
