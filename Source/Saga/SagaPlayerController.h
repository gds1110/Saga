// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "Data/CharacterStuructData.h"
#include "InputActionValue.h"
#include "SagaPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

// 플레이어 컨트롤러의 다양한 모드를 정의하는 열거형
UENUM(BlueprintType)
enum class EControllerMode : uint8
{
	Default UMETA(DisplayName = "Mode 1"),
	SetStart UMETA(DisplayName = "Mode 2"),
	SetTarget UMETA(DisplayName = "Mode 3"),
	SetCharacterPosition UMETA(DisplayName = "Mode 4"),
	Max UMETA(Hidden)
};

UCLASS()
class ASagaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASagaPlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputConfigData* InputActions;

	UPROPERTY(EditAnywhere,Category=Highlight)
	TObjectPtr<class AGridHighlight> Highlight;
	UPROPERTY(EditAnywhere)
	FCharacterStuructData CurrentData;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> PreviewStaticMesh;

	UPROPERTY(EditAnywhere, Category = CursorUnderGrid)
	TObjectPtr<class AGrid> CursorUnderGrid;

	UPROPERTY(EditAnywhere,Category=State)
	EControllerMode ControlMode;

	UPROPERTY(EditAnywhere, Category = GridManager)
	TObjectPtr<class AGridManager> GridManager;

	UPROPERTY(EditAnywhere,Category=CurrentCharacter)
	TObjectPtr<class ACharacterBase> CurrentCharacter;

	UPROPERTY(EditAnywhere, Category = SelectGrid)
	TObjectPtr<class AGrid> SelectedGrid;

	UPROPERTY()
	TArray<TObjectPtr<class AGrid>> CurrentPath;

	UPROPERTY(VisibleAnywhere, Category=SpellHighlight)
	TArray<TObjectPtr<class AGrid>> CandidateGrids;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class APreviewActor> PreviewActor;

	UPROPERTY(EditAnywhere,Category=UIWidget)
	TObjectPtr<class UCharacterSpawnScreen> SpawnScreenUI;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=UIWidgetClass)	
	TSubclassOf<class UCharacterSpawnScreen> SpawnScreenUIClass;

	UPROPERTY(EditAnywhere, Category = UIWidget)
	TObjectPtr<class UMainHudUI> CharacterHudUI;

	UPROPERTY()
	TObjectPtr<class USagaWorldSubSystem> WorldSubsystem;

	UPROPERTY()
	TObjectPtr<class ASagaGameMode> CurrentGameMode;

	UPROPERTY()
	TObjectPtr<class UUserWidget> CurrentWidgetScreen;

	UPROPERTY(EditAnywhere)
	bool bIsInArea;

	virtual void PlayerTick(float DeltaTime) override;

	void InitGridManager(class AGridManager* gridManager);

	void SetPreviewStaticMeshComponent();
	void ChangeStructData(FCharacterStuructData InData);

	UPROPERTY()
	TArray<FIntPoint> HighlightPositions;
	UPROPERTY(VisibleAnywhere, Category = EffectPool)
	TObjectPtr<class AEffectManager> EffectManager;
protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay();

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();

	void OnSetGridTriggered();
	void CameraMove(const FInputActionValue& Value);
	void CameraRotate(const FInputActionValue& Value);
	void ZoomInOut(const FInputActionValue& Value);
	void ChangeMode();
	void ChangeMode(EControllerMode ConMode);
	void StartPathFind();
	void StartPathFind(class AGrid* grid);
	void StartPathFind(class ACharacterBase* InputCharacter);
	void RotateCamButton(const FInputActionValue& Value);
	void RotateRightCam();
	void RotateLeftCam();

	void MoveCam(FVector NewVector);
private:
	FVector CachedDestination;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed

	TObjectPtr<class USpringArmComponent> CameraBoom;
	TObjectPtr<class ACameraPawn> MyCameraPawn;

public:
	void HighlightOnRange(TArray<class AGrid*> RangeGrids);

	//Object PoolingHighlight
public:
		class AGridHighlight* GetPoolObject();
		void ExpandPool();
		void ClearPool();
		void ReturnOjbect(class AGridHighlight* ReturnObject);
public:
		TSubclassOf<class AGridHighlight> HighlightObjectClass;
		int32 PoolSize = 0;
		int32 ExpandSize = 10;
private:
		TArray<class AGridHighlight*> HightlightPool;
		TArray<class AGridHighlight*> OutedHighlightPool;

public:
	void ChangeUIMode();

	void AiModePlayerController();
	void AiModeClickEvent();
	void PlayingModePlayerController();
	void PlayingModeClickEvent();
	void SimulationModePlayerController();
	void SimulationModeClickEvent();
};


