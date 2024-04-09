// Copyright Epic Games, Inc. All Rights Reserved.

#include "SagaPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "SagaCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Grid/GridHighlight.h"
#include "Interface/GridActorInterface.h"
#include "Camera/CameraPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Grid/Grid.h"
#include "Grid/GridManager.h"
#include <SagaGameMode.h>
#include "Kismet/GameplayStatics.h"
#include "Data/Input/InputConfigData.h"
#include "Math/Rotator.h"
#include "Character/CharacterBase.h"
#include "Preview/PreviewActor.h"
#include "UI/CharacterSpawn/CharacterSpawnScreen.h"
#include "SubSystem/SagaWorldSubSystem.h"
#include "UI/InGameUI/MainHudUI.h"
#include "Camera/CameraActor.h"
#include "AIController.h"
#include "AStar/AstarActorComponent.h"
#include "Effect/EffectManager.h"
#include "Turn/TurnManager.h"

ASagaPlayerController::ASagaPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;

	Highlight = CreateDefaultSubobject<AGridHighlight>(TEXT("Highlight"));
	ControlMode = EControllerMode::Default;

	


}

void ASagaPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);


	Highlight = GetWorld()->SpawnActor<AGridHighlight>(AGridHighlight::StaticClass());
	Highlight->SetActorHiddenInGame(true);
	PreviewActor = GetWorld()->SpawnActor<APreviewActor>(APreviewActor::StaticClass());
	PreviewActor->SetActorHiddenInGame(true);
	ACameraPawn* sagaChar = Cast<ACameraPawn>(GetPawn());
	if (sagaChar != NULL)
	{
		CameraBoom = sagaChar->GetCameraBoom();
		MyCameraPawn = sagaChar;
	}

	ASagaGameMode* gameMode = Cast<ASagaGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode)
	{
		GridManager = gameMode->GetGridManager();
		CurrentGameMode = gameMode;
	}
	SpawnScreenUI = gameMode->SpawnScreenUI;
	CharacterHudUI = gameMode->CharacterHudUI;
	EffectManager = gameMode->EffectManager;

	CurrentWidgetScreen = SpawnScreenUI;
	CurrentWidgetScreen->SetVisibility(ESlateVisibility::Visible);
	WorldSubsystem = GetWorld()->GetSubsystem<USagaWorldSubSystem>();
	if (WorldSubsystem)
	{

	}
	//ChangeMode(EControllerMode::SetCharacterPosition);
	ControlMode = EControllerMode::SetCharacterPosition;
}

void ASagaPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	if (GridManager == nullptr)
	{
		ASagaGameMode* gameMode = Cast<ASagaGameMode>(GetWorld()->GetAuthGameMode());
		if (gameMode)
		{
			GridManager = gameMode->GetGridManager();
		}
		return;
	}
	
	if (ControlMode == EControllerMode::SetCharacterPosition)
	{
		FHitResult Hit;
		bool bHitSuccessful = false;
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

		if (bHitSuccessful)
		{


			AGrid* TempActor = Cast<AGrid>(Hit.GetActor());
			if (TempActor != CursorUnderGrid && TempActor != nullptr)
			{
				ClearPool();
				GridManager->RefreshGrids();
				Highlight->SetActorHiddenInGame(false);
				PreviewActor->SetActorHiddenInGame(false);
				bIsInArea = true;
				CursorUnderGrid = TempActor;

				if (Highlight != nullptr)
				{
					if (TempActor->GetTileState() != EGridState::INVALID)
					{
						Highlight->SetActorLocation(CursorUnderGrid->GetGridLocation_Implementation() + FVector(0, 0, 0.1f));
						if (PreviewActor != nullptr)
						{
							PreviewActor->SetActorLocation(CursorUnderGrid->GetGridLocation_Implementation() + FVector(0, 0, 0.1f));
						}
					}
				}

			}

		}
		else
		{
			Highlight->SetActorHiddenInGame(true);
			PreviewActor->SetActorHiddenInGame(true);

			bIsInArea = false;
		}


		return;
	}

	if (CurrentGameMode != nullptr)
	{
		switch (CurrentGameMode->CurrentGameMode)
		{
		case ECurrentGameMode::AIMODE:
			AiModePlayerController();
			break;
		case ECurrentGameMode::PLAYINGMODE:
			PlayingModePlayerController();
			break;
		case ECurrentGameMode::SIMULMODE:
			SimulationModePlayerController();
			break;
		default:
			break;
		}
	}

}

void ASagaPlayerController::SetPreviewStaticMeshComponent()
{
	if (CurrentData.D_StaticMesh != NULL)
	{

		PreviewActor->ChangeStaticMesh(CurrentData.D_StaticMesh);
		PreviewActor->SetActorHiddenInGame(true);
	}
	
}

void ASagaPlayerController::ChangeStructData(FCharacterStuructData InData)
{
	CurrentData = InData;
	SetPreviewStaticMeshComponent();
}



void ASagaPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();


	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}



	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		
		EnhancedInputComponent->BindAction(InputActions->SetDestinationClickAction, ETriggerEvent::Completed, this, &ASagaPlayerController::OnSetGridTriggered); //좌클릭

		EnhancedInputComponent->BindAction(InputActions->ChangeModeAction, ETriggerEvent::Completed, this, &ASagaPlayerController::ChangeMode);
		EnhancedInputComponent->BindAction(InputActions->ZoomInOutAction, ETriggerEvent::Triggered, this, &ASagaPlayerController::ZoomInOut);
		EnhancedInputComponent->BindAction(InputActions->CamMoveAction, ETriggerEvent::Triggered, this, &ASagaPlayerController::CameraMove);
		//EnhancedInputComponent->BindAction(InputActions->CamRotateAction, ETriggerEvent::Triggered, this, &ASagaPlayerController::CameraRotate);
		EnhancedInputComponent->BindAction(InputActions->PathFindAction, ETriggerEvent::Completed, this, &ASagaPlayerController::StartPathFind);
		EnhancedInputComponent->BindAction(InputActions->CamRotateButtonAction, ETriggerEvent::Triggered, this, &ASagaPlayerController::RotateCamButton);
		

	}
}

void ASagaPlayerController::OnInputStarted()
{
	StopMovement();
}
/*
// Triggered every frame when the input is held down
void ASagaPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;

		AGrid* TempGrid = Cast<AGrid>(Hit.GetActor());
		if (TempGrid != nullptr)
		{
			if (CursorUnderGrid == TempGrid)
			{
				if (SelectedGrid != TempGrid)
				{
					GridManager->StartPathFind(SelectedGrid);
				}
					SelectedGrid = TempGrid;
			}
		}
		if (ControlMode == EControllerMode::Default)
		{
			
			if (CursorUnderGrid)
			{
				
			}
		}
	}
	
}

void ASagaPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void ASagaPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void ASagaPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}
*/
void ASagaPlayerController::OnSetGridTriggered()
{
	
	if (ControlMode == EControllerMode::SetCharacterPosition)
	{
		if (CurrentData.D_CharacterClass != nullptr)
		{
			if (CursorUnderGrid != nullptr && PreviewActor->CheckMesh())
			{
				GridManager->SpawnOnGrid(CursorUnderGrid, CurrentData);
				if (CharacterHudUI != nullptr)
				{
					//CharacterHudUI->AddTurnOrder(CursorUnderGrid);
					CurrentGameMode->UpdateCharacterList();
				}

			}
			PreviewActor->ChangeStaticMesh(nullptr);
		}
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Clickeddd"));
	if (CurrentGameMode != nullptr)
	{
		switch (CurrentGameMode->CurrentGameMode)
		{
		case ECurrentGameMode::AIMODE:
			AiModeClickEvent();
			break;
		case ECurrentGameMode::PLAYINGMODE:
			PlayingModeClickEvent();
			break;
		case ECurrentGameMode::SIMULMODE:
			SimulationModeClickEvent();
			break;
		default:
			break;
		}
	}


}

void ASagaPlayerController::CameraMove(const FInputActionValue& Value)
{
	FVector2D moveVector = Value.Get<FVector2d>();
	
	FVector addLocation = FVector(moveVector.Y * 50,moveVector.X*50,0);
	MoveCam(addLocation);



}

void ASagaPlayerController::CameraRotate(const FInputActionValue& Value)
{
	FVector2d value = Value.Get<FVector2d>();
	FRotator inputRoator = FRotator(0, value.Y*4.0, value.X*4.0);
	
	FRotator MyControlRotation = GetControlRotation();
	MyCameraPawn->SetActorRotation(FRotator(0,0,ControlRotation.Yaw));

	FRotator NewRotator = FRotator(ControlRotation.Roll, inputRoator.Pitch + ControlRotation.Pitch,
		inputRoator.Yaw + ControlRotation.Yaw);
	SetControlRotation(NewRotator);


}

void ASagaPlayerController::ZoomInOut(const FInputActionValue& Value)
{

	float inputValue = Value.Get<float>();
	MyCameraPawn->Zoom(inputValue);
}



void ASagaPlayerController::ChangeMode()
{
	GridManager->ClearPathStateGrids();
	GridManager->RefreshGrids();

	FString DisplayText;
	if (CurrentWidgetScreen)
	{
		CurrentWidgetScreen->SetVisibility(ESlateVisibility::Hidden);
	}
	switch (ControlMode)
	{
	case EControllerMode::Default:
		ControlMode = EControllerMode::SetStart;
		DisplayText = TEXT("Current Mode: SetStart");
		break;
	case EControllerMode::SetStart:
		ControlMode = EControllerMode::SetTarget;
		DisplayText = TEXT("Current Mode: SetTarget");

		break;
	case EControllerMode::SetTarget:
		ControlMode = EControllerMode::SetCharacterPosition;
		DisplayText = TEXT("Current Mode: SetCharacterPosition");
		
		if (SpawnScreenUI)
		{
			CurrentWidgetScreen = SpawnScreenUI;
		}
		else
		{

			ASagaGameMode* MyGameMode = Cast<ASagaGameMode>(GetWorld()->GetAuthGameMode());
			if (MyGameMode)
			{
				SpawnScreenUI = MyGameMode->SpawnScreenUI;
				CurrentWidgetScreen = SpawnScreenUI;
			}
		}
		break;
	case EControllerMode::SetCharacterPosition:
		ControlMode = EControllerMode::Default;
		DisplayText = TEXT("Current Mode: Default");
		//SpawnScreenUI->SetVisibility(ESlateVisibility::Hidden);
		CurrentWidgetScreen = CharacterHudUI;
	case EControllerMode::Max:
		break;
	default:
		break;
	}
	if (CurrentWidgetScreen)
	{
		CurrentWidgetScreen->SetVisibility(ESlateVisibility::Visible);
	}
	// 화면에 Display Name 표시
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DisplayText);
	}

}

void ASagaPlayerController::ChangeMode(EControllerMode ConMode)
{
	if (GridManager != nullptr)
	{
		GridManager->ClearPathStateGrids();
		GridManager->RefreshGrids();
	}
	FString DisplayText;
	if (CurrentWidgetScreen)
	{
		CurrentWidgetScreen->SetVisibility(ESlateVisibility::Hidden);
	}
	ControlMode = ConMode;
	switch (ConMode)
	{
	case EControllerMode::Default:
		DisplayText = TEXT("Current Mode: Default");
		//SpawnScreenUI->SetVisibility(ESlateVisibility::Hidden);
		CurrentWidgetScreen = CharacterHudUI;
		break;
	case EControllerMode::SetStart:
		DisplayText = TEXT("Current Mode: Do Spell");
		CurrentWidgetScreen = CharacterHudUI;

		break;
	case EControllerMode::SetTarget:
		DisplayText = TEXT("Current Mode: Do Move");

		CurrentWidgetScreen = CharacterHudUI;

		break;
	case EControllerMode::SetCharacterPosition:
		DisplayText = TEXT("Current Mode: SetCharacterPosition");

		if (SpawnScreenUI)
		{
			CurrentWidgetScreen = SpawnScreenUI;
		}
		else
		{

			ASagaGameMode* MyGameMode = Cast<ASagaGameMode>(GetWorld()->GetAuthGameMode());
			if (MyGameMode)
			{
				SpawnScreenUI = MyGameMode->SpawnScreenUI;
				CurrentWidgetScreen = SpawnScreenUI;
			}
		}
		break;
	case EControllerMode::Max:
		break;
	default:
		break;
	}

	if (CurrentWidgetScreen)
	{
		CurrentWidgetScreen->SetVisibility(ESlateVisibility::Visible);
	}
	// 화면에 Display Name 표시
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DisplayText);
	}

}

void ASagaPlayerController::StartPathFind()
{
	if (GridManager)
	{
		GridManager->StartPathFind();
		GridManager->GridOnOffDelegate.Broadcast(FIntPoint(6, 6), false);
	}
}

void ASagaPlayerController::StartPathFind(AGrid* grid)
{
	/*if (CurrentCharacter)
	{
		int32 stamina = CurrentCharacter->GetCharacterStamina();
		GridManager->StartPathFind(grid, stamina);
	}*/
	UE_LOG(LogTemp, Log, TEXT("StartPathFind"));
	if (grid->OnCharacter != nullptr)
	{
		
		GridManager->StartPathFind(grid->OnCharacter);
	}
	
}

void ASagaPlayerController::StartPathFind(ACharacterBase* InputCharacter)
{
	if (CurrentCharacter)
	{
		int32 stamina = CurrentCharacter->GetCharacterStamina();
		GridManager->StartPathFind(InputCharacter);
	}
}

void ASagaPlayerController::RotateCamButton(const FInputActionValue& Value)
{
	float value = Value.Get<float>();
	if (value > 0)
	{
		MyCameraPawn->RotateLeft();
	}
	else
	{
		MyCameraPawn->RotateRight();
	}
}

void ASagaPlayerController::RotateRightCam()
{
}

void ASagaPlayerController::RotateLeftCam()
{

}

void ASagaPlayerController::MoveCam(FVector NewVector)
{
	const FRotator myRotator = GetPawn()->GetActorRotation();
	FVector result = myRotator.RotateVector(NewVector);
	result = FVector(result.X, result.Y, 0);
	GetPawn()->SetActorLocation(GetPawn()->GetActorLocation() + result);

}

void ASagaPlayerController::HighlightOnRange(TArray<class AGrid*> RangeGrids)
{
	for (AGrid* InGrid : RangeGrids)
	{
		AGridHighlight* tempHL = GetPoolObject();
		tempHL->SetActorLocation(InGrid->GetGridLocation_Implementation() + FVector(0, 0, 0.1f));
		tempHL->SetActive(true);
	}
}

AGridHighlight* ASagaPlayerController::GetPoolObject()
{
	if (HightlightPool.Num() == 0)
	{
		ExpandPool();
	}
	AGridHighlight* TempHL = HightlightPool.Pop();
	OutedHighlightPool.Push(TempHL);
	return TempHL;

}

void ASagaPlayerController::ExpandPool()
{
	for (int i = 0; i < ExpandSize; i++)
	{
		AGridHighlight* PoolableHightlight = GetWorld()->SpawnActor<AGridHighlight>(AGridHighlight::StaticClass(),
			FVector().ZeroVector, FRotator().ZeroRotator);
		PoolableHightlight->SetObjectPool(this);
		PoolableHightlight->SetActive(false);
		HightlightPool.Push(PoolableHightlight);
	}
	PoolSize += ExpandSize;
}

void ASagaPlayerController::ClearPool()
{
	for (AGridHighlight* poolob : OutedHighlightPool)
	{
		poolob->ReturnSelf();
	}
	OutedHighlightPool.Empty();
}

void ASagaPlayerController::ReturnOjbect(AGridHighlight* ReturnObject)
{
	HightlightPool.Push(ReturnObject);
}
void ASagaPlayerController::ChangeUIMode()
{
	if (CurrentWidgetScreen)
	{
		CurrentWidgetScreen->SetVisibility(ESlateVisibility::Hidden);
	}
	if (CurrentWidgetScreen == SpawnScreenUI)
	{
		CurrentWidgetScreen = CharacterHudUI;
	}
	else
	{
		CurrentWidgetScreen = SpawnScreenUI;
	}
	if (CurrentWidgetScreen)
	{
		CurrentWidgetScreen->SetVisibility(ESlateVisibility::Visible);
	}

}
//////////////////////////////////////////////////// Mouse Move and Event
void ASagaPlayerController::AiModePlayerController()
{
	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "InBox");

	if (bHitSuccessful)
	{


		AGrid* TempActor = Cast<AGrid>(Hit.GetActor());
		if (TempActor != CursorUnderGrid && TempActor != nullptr)
		{
			ClearPool();
			GridManager->RefreshGrids();
			//GridManager->ClearAllGrids();
			Highlight->SetActorHiddenInGame(false);
			PreviewActor->SetActorHiddenInGame(false);
			bIsInArea = true;
			CursorUnderGrid = TempActor;
			//CandidateGrids.Empty();

			if (Highlight != nullptr)
			{
				if (TempActor->GetTileState() != EGridState::INVALID)
				{
					Highlight->SetActorLocation(CursorUnderGrid->GetGridLocation_Implementation() + FVector(0, 0, 0.1f));
					FIntPoint tiletogrid = GridManager->LocationToTile(CursorUnderGrid->GetGridLocation_Implementation());

					if (PreviewActor != nullptr)
					{
						PreviewActor->SetActorLocation(CursorUnderGrid->GetGridLocation_Implementation() + FVector(0, 0, 0.1f));
						//PreviewActor->SetActorHiddenInGame(false);
					}
				}
			}

		}

	}
	else
	{
		Highlight->SetActorHiddenInGame(true);
		PreviewActor->SetActorHiddenInGame(true);

		bIsInArea = false;
	}





}

void ASagaPlayerController::AiModeClickEvent()
{
	//AI모드 클릭이벤트
	if (CursorUnderGrid != nullptr)
	{
		//GridManager->ClearAllGrids();
		//SelectedGrid = CursorUnderGrid;
		//CurrentCharacter = SelectedGrid->GetOnCharacter();
		//CharacterHudUI->ChangeInfo(CursorUnderGrid);
	}

}

void ASagaPlayerController::PlayingModePlayerController()
{
	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "InBox");

	if (bHitSuccessful)
	{


		AGrid* TempActor = Cast<AGrid>(Hit.GetActor());
		if (TempActor != CursorUnderGrid && TempActor != nullptr)
		{
			ClearPool();
			GridManager->RefreshGrids();
			//GridManager->ClearAllGrids();
			Highlight->SetActorHiddenInGame(false);
			PreviewActor->SetActorHiddenInGame(false);
			bIsInArea = true;
			CursorUnderGrid = TempActor;
			//CandidateGrids.Empty();

			if (Highlight != nullptr)
			{
				if (TempActor->GetTileState() != EGridState::INVALID)
				{
					Highlight->SetActorLocation(CursorUnderGrid->GetGridLocation_Implementation() + FVector(0, 0, 0.1f));
					FIntPoint tiletogrid = GridManager->LocationToTile(CursorUnderGrid->GetGridLocation_Implementation());

					if (PreviewActor != nullptr)
					{
						PreviewActor->SetActorLocation(CursorUnderGrid->GetGridLocation_Implementation() + FVector(0, 0, 0.1f));
					}
				}
			}

			if (CursorUnderGrid->bIsCandidateTarget == true && CurrentCharacter != nullptr)
			{
				CandidateGrids = GridManager->FindBFS(CursorUnderGrid, CurrentCharacter->CurrentSpell);
				HighlightOnRange(CandidateGrids);
			}

				if (CursorUnderGrid != nullptr && CurrentCharacter != nullptr)
				{
					if (CurrentCharacter->CurState == ECharacterState::MOVEREADY)
					{

						GridManager->ClearPathStateGrids();
						CurrentPath = GridManager->GetAstartCompo()->GetPathFInd(CurrentCharacter->GetCharacterGrid(), CursorUnderGrid);
						GridManager->RefreshGrids();
					}
				}
			
		}

	}
	else
	{
		Highlight->SetActorHiddenInGame(true);
		PreviewActor->SetActorHiddenInGame(true);

		bIsInArea = false;
	}

}

void ASagaPlayerController::PlayingModeClickEvent()
{
	//플레이모드 클릭이벤트

	if (CurrentGameMode->TurnManager->CurrentTurnCharacter->PlayerType != EPlayerType::PLAYER)
	{
		return;
	}



	if (CurrentCharacter->PlayerType != EPlayerType::ENEMY)
	{
		if (CursorUnderGrid != nullptr)
		{
			SelectedGrid = CursorUnderGrid;
		}
		if (CursorUnderGrid != nullptr && CurrentCharacter == nullptr)
		{
			GridManager->ClearAllGrids();
			CurrentCharacter = SelectedGrid->GetOnCharacter();
			CharacterHudUI->ChangeInfo(CursorUnderGrid);
		}

		if (CurrentCharacter->PlayerType != EPlayerType::ENEMY)
		{
			switch (CurrentCharacter->CurState)
			{
			case ECharacterState::MOVEREADY:
				if (CursorUnderGrid != SelectedGrid)
				{
					SelectedGrid = nullptr;
					GridManager->GridOnOffDelegate.Broadcast(FIntPoint(-1, -1), false);
					GridManager->ClearAllGrids();
					if (CurrentPath.Num() > 0)
					{
						CurrentCharacter->AddCommands(CommandMove, CurrentPath);
						CurrentCharacter->DoCommand();
					}
					GridManager->ClearPathStateGrids();
					GridManager->RefreshGrids();
					CurrentPath.Reset();
					CurrentCharacter->CurState = ECharacterState::WAIT;

				}
				break;
			case ECharacterState::SPELLREADY:
				if (CursorUnderGrid->bIsCandidateTarget == true && CandidateGrids.Num() > 0)
				{
					CurrentCharacter->DoSpell(CandidateGrids);
					CurrentCharacter->DoCommand();
					CurrentCharacter->CurState = ECharacterState::WAIT;
				}
				break;
			case ECharacterState::WAIT:


				break;
			case ECharacterState::MAX:

				break;
			default:
				break;
			}

		}

	}


	//switch (ControlMode)
	//{
	//
	//case EControllerMode::SetTarget:
	//	
	//	break;
	//case EControllerMode::SetCharacterPosition:
	//	if (CurrentData.D_CharacterClass != nullptr)
	//	{
	//		if (CursorUnderGrid != nullptr && PreviewActor->CheckMesh())
	//		{
	//			GridManager->SpawnOnGrid(CursorUnderGrid, CurrentData);
	//			if (CharacterHudUI != nullptr)
	//			{
	//				//CharacterHudUI->AddTurnOrder(CursorUnderGrid);
	//				CurrentGameMode->UpdateCharacterList();
	//			}

	//		}
	//		PreviewActor->ChangeStaticMesh(nullptr);
	//	}

	//	break;
	//case EControllerMode::Max:
	//	break;
	//default:
	//	break;
	//}


}

void ASagaPlayerController::SimulationModePlayerController()
{

	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);


	if (bHitSuccessful)
	{


		AGrid* TempActor = Cast<AGrid>(Hit.GetActor());
		if (TempActor != CursorUnderGrid && TempActor != nullptr)
		{
			ClearPool();
			GridManager->RefreshGrids();
			//GridManager->ClearAllGrids();
			Highlight->SetActorHiddenInGame(false);
			PreviewActor->SetActorHiddenInGame(false);
			bIsInArea = true;
			CursorUnderGrid = TempActor;
			//CandidateGrids.Empty();

			if (Highlight != nullptr)
			{
				if (TempActor->GetTileState() != EGridState::INVALID)
				{
					Highlight->SetActorLocation(CursorUnderGrid->GetGridLocation_Implementation() + FVector(0, 0, 0.1f));
					FIntPoint tiletogrid = GridManager->LocationToTile(CursorUnderGrid->GetGridLocation_Implementation());

					if (PreviewActor != nullptr)
					{
						PreviewActor->SetActorLocation(CursorUnderGrid->GetGridLocation_Implementation() + FVector(0, 0, 0.1f));
					}
				}
			}
			if (CurrentCharacter != nullptr)
			{
				if (CurrentCharacter->CurState == ECharacterState::SPELLREADY)
				{
					if (CursorUnderGrid->bIsCandidateTarget == true && CurrentCharacter != nullptr)
					{
						//CandidateGrids = GridManager->FindBFS(CursorUnderGrid);
						CandidateGrids = GridManager->FindBFS(CursorUnderGrid, CurrentCharacter->CurrentSpell,CurrentCharacter->GetActionArea());
						HighlightOnRange(CandidateGrids);
					}
				}
				if (CurrentCharacter->CurState == ECharacterState::MOVEREADY)
				{
					if (CursorUnderGrid != nullptr && CurrentCharacter != nullptr)
					{

						GridManager->ClearPathStateGrids();
						CurrentPath = GridManager->GetAstartCompo()->GetPathFInd(CurrentCharacter->GetCharacterGrid(), CursorUnderGrid);
						GridManager->RefreshGrids();

					}
				}
			}

		}

	}
	else
	{
		Highlight->SetActorHiddenInGame(true);
		PreviewActor->SetActorHiddenInGame(true);

		bIsInArea = false;
	}




}

void ASagaPlayerController::SimulationModeClickEvent()
{
	//시뮬레이션 클릭이벤트
	
	if (CursorUnderGrid != nullptr&&CursorUnderGrid!=SelectedGrid)
	{
		SelectedGrid = CursorUnderGrid;
	}

	if(CurrentCharacter!=nullptr)
	{
		
		switch (CurrentCharacter->CurState)
			{
			case ECharacterState::MOVEREADY:
			
					GridManager->GridOnOffDelegate.Broadcast(FIntPoint(-1, -1), false);
					GridManager->ClearAllGrids();				
					
					if (CurrentPath.Num() > 0)
					{
						CurrentCharacter->SetFocusTarget(CurrentPath[CurrentPath.Max()-1]);
						CurrentCharacter->AddCommands(CommandMove, CurrentPath);
						CurrentCharacter->DoCommand();
					}
					GridManager->ClearPathStateGrids();
					GridManager->RefreshGrids();
					CurrentPath.Reset();
					CurrentCharacter->CurState = ECharacterState::WAIT;
					return;
				
				break;
			case ECharacterState::SPELLREADY:
				if (CursorUnderGrid->bIsCandidateTarget == true && CandidateGrids.Num() > 0)
				{
					CurrentCharacter->DoSpell(CandidateGrids);
					CurrentCharacter->DoCommand();
					CurrentCharacter->CurState = ECharacterState::WAIT;
					return;
				}
				break;
			case ECharacterState::WAIT:


				break;
			case ECharacterState::MAX:

				break;
			default:
				break;
			}
			
		
	}

	if (CursorUnderGrid != nullptr)
	{
		GridManager->ClearAllGrids();
		CurrentCharacter = SelectedGrid->GetOnCharacter();
		CharacterHudUI->ChangeInfo(CursorUnderGrid);
	}





	//switch (ControlMode)
	//{
	//case EControllerMode::Default:
	//	if (CursorUnderGrid != nullptr)
	//	{
	//		if (CursorUnderGrid != SelectedGrid)
	//		{

	//			GridManager->ClearAllGrids();
	//			SelectedGrid = CursorUnderGrid;
	//			CurrentCharacter = SelectedGrid->GetOnCharacter();
	//			CharacterHudUI->ChangeInfo(CursorUnderGrid);
	//		}

	//	}

	//	break;
	//case EControllerMode::SetStart:
	//	if (CursorUnderGrid->bIsCandidateTarget == true && CandidateGrids.Num() > 0)
	//	{
	//		if (CurrentCharacter != nullptr)
	//		{
	//			//CurrentCharacter->SetFocusTarget(CandidateGrids[0]);
	//			CurrentCharacter->DoSpell(CandidateGrids);
	//			CurrentCharacter->DoCommand();

	//		}
	//	}
	//	break;
	//case EControllerMode::SetTarget:
	//	if (CursorUnderGrid != SelectedGrid)
	//	{
	//		SelectedGrid = nullptr;
	//		GridManager->GridOnOffDelegate.Broadcast(FIntPoint(-1, -1), false);
	//		GridManager->ClearAllGrids();
	//		if (CurrentCharacter)
	//		{
	//			//TODO
	//			//커맨드 추가하면 그 커맨드 따라 움직이게하기.
	//		/*	CurrentCharacter->SetPathToFollow(CurrentPath);
	//			CurrentCharacter->MoveCharacter();*/

	//			if (CurrentPath.Num() > 0)
	//			{
	//				//CurrentCharacter->SetFocusTarget(CurrentPath[CurrentPath.Max()-1]);
	//				CurrentCharacter->AddCommands(CommandMove, CurrentPath);
	//				CurrentCharacter->DoCommand();
	//			}
	//			GridManager->ClearPathStateGrids();
	//			GridManager->RefreshGrids();
	//			CurrentPath.Reset();
	//		}
	//	}
	//	break;
	//case EControllerMode::SetCharacterPosition:
	//	if (CurrentData.D_CharacterClass != nullptr)
	//	{
	//		if (CursorUnderGrid != nullptr && PreviewActor->CheckMesh())
	//		{
	//			GridManager->SpawnOnGrid(CursorUnderGrid, CurrentData);
	//			if (CharacterHudUI != nullptr)
	//			{
	//				//CharacterHudUI->AddTurnOrder(CursorUnderGrid);
	//				CurrentGameMode->UpdateCharacterList();
	//			}

	//		}
	//		PreviewActor->ChangeStaticMesh(nullptr);
	//	}

	//	break;
	//case EControllerMode::Max:
	//	break;
	//default:
	//	break;
	//}

}
