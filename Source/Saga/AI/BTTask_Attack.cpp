// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AI/SAAI.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Grid/Grid.h"
#include "Grid/GridManager.h"
#include "AStar/AstarActorComponent.h"
#include <SagaGameMode.h>

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("DoAttack");

}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	auto ControllingPawn = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed; // 抗寇贸府. 	
	ACharacterBase* TargetActor = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == TargetActor) return EBTNodeResult::Failed; // 抗寇贸府. 	

	TArray<TObjectPtr<class AGrid>> CandidateGrids;

	CandidateGrids = ControllingPawn->GetGridManager()->FindBFS(TargetActor->GetCharacterGrid(), ControllingPawn->CurrentSpell,ControllingPawn->GetActionArea());
	if (CandidateGrids.Num() > 0)
	{
		ControllingPawn->FocusTarget(CandidateGrids[0]);
		ControllingPawn->DoSpell(CandidateGrids);
		ControllingPawn->DoCommand();
		ControllingPawn->AddStatValue(ECharValue::MP, -(ControllingPawn->CurrentSpell.D_CostMana));
	}
	for (AGrid* InGrid : CandidateGrids)
	{
		InGrid->bIsCandidateTarget = false;
	}
	ControllingPawn->GetGridManager()->ClearAllGrids();


	return EBTNodeResult::Succeeded;
}
