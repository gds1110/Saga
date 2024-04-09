// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTS_FindTarget.h"
#include "AI/SAAI.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CharacterBase.h"
#include "Grid/Grid.h"
#include "Grid/GridManager.h"
#include "AStar/AstarActorComponent.h"
#include "AI/SpawnAIController.h"
#include <SagaGameMode.h>
#include "Kismet/GameplayStatics.h"



UBTS_FindTarget::UBTS_FindTarget()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTS_FindTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	//범위 내에서 적 찾기
	if (CurGameMode == nullptr)
	{
		CurGameMode = Cast<ASagaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	}
	if (CurGameMode == nullptr)
	{
		return;
	}
	ACharacterBase* ControllingPawn = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return;
	}
	if (OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET) != nullptr)
	{
		return;
	}
	if (CurGameMode) {

		TArray<ACharacterBase*> AllCharacters = CurGameMode->AllCharacterList;
		ACharacterBase* TargetActor = nullptr;
		float ClosestDistanceSquared = TNumericLimits<float>::Max();
		for (ACharacterBase* Char : AllCharacters)
		{
			if (Char == nullptr || Char->IsDead == true)
			{
				continue;
			}
			float DistanceSquared = FVector::DistSquared(Char->GetActorLocation(), ControllingPawn->GetActorLocation());
			if (Char->PlayerType != ControllingPawn->PlayerType)
			{
				if (DistanceSquared < ClosestDistanceSquared)
				{
					ClosestDistanceSquared = DistanceSquared;
					TargetActor = Char;
				}
			}
		}
		if (TargetActor != nullptr)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, TargetActor);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGETGRID, TargetActor->GetCharacterGrid());
			UE_LOG(LogTemp, Log, TEXT("Find Target"));

		}
	}


}
