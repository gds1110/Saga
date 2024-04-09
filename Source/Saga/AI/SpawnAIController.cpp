// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SpawnAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CharacterBase.h"
#include "Grid/Grid.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Navigation/PathFollowingComponent.h"
#include "AI/Navigation/NavigationTypes.h"
#include "AI/NavigationSystemBase.h"
#include "AI/SAAI.h"
#include "Kismet/KismetMathLibrary.h"
#include "Async/Async.h"
#include "Engine/DamageEvents.h"


ASpawnAIController::ASpawnAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/Saga/AI/BB_Character.BB_Character'"));
	if (BBAssetRef.Object != nullptr)
	{
		BBAsset = BBAssetRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Saga/AI/BT_Character.BT_Character'"));
	if (BTAssetRef.Object != nullptr)
	{
		BTAsset = BTAssetRef.Object;
	}
	PrimaryActorTick.bCanEverTick = true;
	CurrentIndex = 0;
	bIsAction = false;
	bIsMoving = false;
}

void ASpawnAIController::BeginPlay()
{

	Super::BeginPlay();

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);


}

void ASpawnAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (OwningCharacter)
	{
		if (bIsAction == true)
		{
			CommandAction();
		}
		//if (CommandDataArray.Num() > 0)
		//{
		//	CommandAction();
		//}
		if (FocusTarget != nullptr)
		{
			SetFocus(FocusTarget);
			//UE_LOG(LogTemp, Log, TEXT("SetFocusing"));
		}
	}
}

void ASpawnAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		bool RunResult = RunBehaviorTree(BTAsset);
	}
	
	if (!Blackboard)
	{
		// 블랙보드 컴포넌트가 유효하지 않은 경우
		UE_LOG(LogTemp, Log, TEXT("Failed to read BB Board"));

		return;
	}

	// 블랙보드에서 값 읽어오기
	
	FName KeyName = FName("IsMyTurn");

	if (Blackboard->GetValueAsBool(KeyName))
	{
		// 성공적으로 값을 읽어왔을 경우 bValue에 원하는 값이 들어있음
		UE_LOG(LogTemp, Log, TEXT("Value of key found"));
	}
	else
	{
		// 값 읽기 실패 시 처리
		UE_LOG(LogTemp, Log, TEXT("Failed to read value for key"));
	}

	//Blackboard->SetValueAsBool(BBKEY_BTURN, true);

}

void ASpawnAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}

}

void ASpawnAIController::StartCommandAction()
{
	if (bIsAction == true)
	{
		return;
	}
	bIsAction = true;
	//CurrentCommandData = CommandDataArray.Pop();
	if (CommandDataArray.Num() > 0)
	{
		CurrentCommandData = CommandDataArray[0];
	}
}

void ASpawnAIController::CommandMove(const FCommandData InCommandData)
{
	//MoveToLocation(InCommandData.Location,0.1f);
	CurrentCommandData = InCommandData;
	if (InCommandData.Type == ECommandType::CommandMove)
	{
		if (CurrentCommandData.CommandGrid != nullptr)
		{
			float DistanceToTarget = FVector::Dist(OwningCharacter->GetActorLocation(),
				CurrentCommandData.CommandGrid->GetGridLocation_Implementation());
			UE_LOG(LogTemp, Log, TEXT("Current Distance : %f"), DistanceToTarget);
			if (DistanceToTarget <= 115.0f)
			{
				CompleteCommandAction();
				return;
			}
		}
		if (InCommandData.CommandGrid != nullptr)
		{
			MoveToLocation(InCommandData.CommandGrid->GetActorLocation());
		}
	}
	
	//if (InCommandData.CommandGrid != nullptr)
	//{
	//
	//		FAIMoveRequest MoveRequest;
	//		MoveRequest.SetGoalLocation(InCommandData.CommandGrid->GetActorLocation());
	//		MoveRequest.SetGoalActor(InCommandData.CommandGrid);
	//		MoveRequest.SetAcceptanceRadius(1.f); // Acceptance radius 설정
	//		MoveRequest.SetUsePathfinding(false);
	//		MoveTo(MoveRequest); // 이동 명령 수행
	//	
	//		if (InCommandData.CommandGrid != nullptr)
	//	{
	//		FAIMoveRequest MoveRequest;
	//		//MoveRequest.SetGoalLocation(InCommandData.CommandGrid->GetActorLocation());
	//		MoveRequest.SetGoalActor(InCommandData.CommandGrid);
	//		MoveRequest.SetAcceptanceRadius(10.f); // Acceptance radius 설정

	//		MoveTo(MoveRequest); // 이동 명령 수행
	//	}
	//}
}

void ASpawnAIController::CommandMove()
{
	if (CurrentCommandData.CommandGrid != nullptr)
	{

		FVector CharacterLocation = OwningCharacter->GetActorLocation();
		FVector CurrentCommandLocation = CurrentCommandData.Location;
		UE_LOG(LogTemp, Log, TEXT("Char Location  X : %f, Y: %f"), CharacterLocation.X, CharacterLocation.Y);
		UE_LOG(LogTemp, Log, TEXT("CurrentCommandLocation Location  X : %f, Y: %f"), CurrentCommandLocation.X, CurrentCommandLocation.Y);


		MoveToActor(CurrentCommandData.CommandGrid);

		float DistanceToTarget = FVector::DistXY(CharacterLocation,
			CurrentCommandLocation);
		if (DistanceToTarget < 50.0f)
		{
			CompleteCommandAction();
		}

		UE_LOG(LogTemp, Log, TEXT("Current Distance : %f, CurrentCommandGrid : %s"), DistanceToTarget, *CurrentCommandData.CommandGrid->GetName());


	}
}

void ASpawnAIController::CommandAction()
{
	
	//bIsAction = false;
	if (CommandDataArray.Num() <= 0)
	{
		bIsAction = false;
		CurrentCommandData = FCommandData();
		return;
	}
	if (CurrentCommandData.IsPlay == true)
	{

	}
	else
	{
		if (CurrentCommandData.Type == ECommandType::CommandMove)
		{
			OwningCharacter->AddStatValue(ECharValue::STAMINA, -10.0f);
		}
		CurrentCommandData.IsPlay = true;
	}
	switch (CurrentCommandData.Type)
	{
	case ECommandType::CommandMove:
		OwningCharacter->bIsMoving = true;
		bIsMoving = true;
		CommandMove();
		break;
	case ECommandType::CommandMoveAttack:
		break;
	case ECommandType::CommandTurn:
		TurnToTarget();
		break;
	case ECommandType::CommandSpell:
		SpellToTarget();
		break;
	case ECommandType::CommandAttack:
		break;
	default:
		break;
	}

	//if (CurrentIndex < CommandDataArray.Num() && !CommandDataArray.IsEmpty())
	//{
	//	ECommandType CmdType = CurrentCommandData.Type;
	//	switch (CmdType)
	//	{
	//	case ECommandType::CommandMove:	
	//		OwningCharacter->bIsMoving = true;
	//		CommandMove(CommandDataArray[CurrentIndex]);	
	//		break;
	//	case ECommandType::CommandMoveAttack:
	//		break;
	//	case ECommandType::CommandTurn:
	//		TurnToTarget();
	//		break;
	//	case ECommandType::CommandSpell:
	//		break;
	//	case ECommandType::CommandAttack:
	//		break;
	//	default:
	//		break;
	//	}

	//	
	//}

}








void ASpawnAIController::TurnToTarget()
{
	
	FVector CurrentLocation = GetPawn()->GetActorLocation();
	FVector TargetLocation = CurrentCommandData.Target->GetActorLocation();
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);

	FRotator CurrentRotation = GetPawn()->GetActorRotation();
	FRotator InterpolatedRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 1.0f);

	GetPawn()->SetActorRotation(InterpolatedRotation);
	if (CurrentRotation.Equals(TargetRotation, 1.0))
	{
		CompleteCommandAction();
		return;
	}
}

void ASpawnAIController::CompleteCommandAction()
{
	//if (CommandDataArray.Num() > 0)
	//{
	//	bIsAction = true;
	//	//CurrentCommandData = CommandDataArray.Pop();
	//
	//}
	//else
	//{

	//	bIsAction = false;
	//}
	if (CurrentCommandData.Type == ECommandType::CommandMove)
	{
		//OwningCharacter->AddStatValue(ECharValue::STAMINA, -10.0f);
		OwningCharacter->SetCharacterGrid(CurrentCommandData.CommandGrid);
		// 이전 그리드에서 캐릭터에 대한 정보삭제필요
	}

	if (CommandDataArray.Num()>0)
	{
		bIsAction = true;
		//CurrentCommandData = CommandDataArray.Pop();
		CommandDataArray.RemoveAt(0);
		if (CommandDataArray.Num() > 0)
		{
			CurrentCommandData = CommandDataArray[0];
		}
		else
		{
			bIsAction = false;
			bIsMoving = false;
			
		}
		//CurrentCommandData = CommandDataArray[0];
	}
	else
	{

		bIsAction = false;
		bIsMoving = false;

	}
	//if(CurrentIndex)
}

void ASpawnAIController::SpellToTarget()
{
	if (CurrentCommandData.Target != nullptr)
	{
		OwningCharacter->AttackToTarget(CurrentCommandData.Target, CurrentCommandData.CommandSpellData.D_Damage);
		FTimerHandle TimerHandle;
		//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASpawnAIController::CompleteCommandAction, 0.2f, false);
		CompleteCommandAction();
	}
}

void ASpawnAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwningCharacter = Cast<ACharacterBase>(InPawn);


	RunAI();
}

void ASpawnAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);


	UPathFollowingComponent* PathFollowingComp = GetPathFollowingComponent();

	//UE_LOG(LogTemp, Log, TEXT("Process Command MOVE and Current Index : %d , TotalNum : %d"),CurrentIndex, CommandDataArray.Num());
	FString EnumAsString = UEnum::GetValueAsString(PathFollowingComp->GetStatus());
	if (Result.IsInterrupted())
	{
		

	}
	else if (Result.IsSuccess())
	{
		/*float DistanceToTarget = FVector::Dist(OwningCharacter->GetActorLocation(),
			CurrentCommandData.CommandGrid->GetGridLocation_Implementation());*/
		/*float DistanceToTarget = FVector::DistXY(OwningCharacter->GetActorLocation(),
			CurrentCommandData.CommandGrid->GetGridLocation_Implementation());*/
		CompleteCommandAction();
		
		//UE_LOG(LogTemp, Log, TEXT("Current Distance : %f, CurrentCommandGrid : %s"), DistanceToTarget, *CurrentCommandData.CommandGrid->GetName());

		//UE_LOG(LogTemp, Log, TEXT("Current Distance : %f, CurrentCommandGrid : %s"), DistanceToTarget, *CurrentCommandData.CommandGrid->GetName());
	}

	//if (CurrentIndex == CommandDataArray.Num() - 1)
	//{
	//	OwningCharacter->bIsMoving = false;
	//	CommandDataArray.Reset();
	//	CurrentIndex = 0;
	//	UE_LOG(LogTemp, Log, TEXT("COMPLETE Command MOVE"));
	//	OnReachedDestination.Broadcast(CurrentCommandData);
	//}
	//
	/*if (CommandDataQueue.Pop() == false)
	{
		OwningCharacter->bIsMoving = false;
	}
	else
	{

	}*/


}

void ASpawnAIController::MoveToCommand()
{
	if (CurrentIndex < CommandDataArray.Num()&&!CommandDataArray.IsEmpty())
	{
		CommandMove(CommandDataArray[CurrentIndex]);
	}
	//if (CommandDataArray.Num() > 0)
	//{
	//	FNavPathSharedPtr CustomPath = FNavigationSystem::CreatePath();
	//	UNavigationSystemV1 nva;
	//	
	//	if (NavigationPath)
	//	{
	//		FAIMoveRequest MoveRequest;
	//		MoveRequest.SetUsePathfinding(false); // 경로 찾기 사용 안 함
	//		MoveRequestSetPath(NavigationPath); // 경로 설정

	//		// 이동 명령 수행
	//		MoveTo(MoveRequest);
	//	}
	//}

}

void ASpawnAIController::TargetFocus(AActor* InFocusTarget)
{
	SetFocus(InFocusTarget);

}
