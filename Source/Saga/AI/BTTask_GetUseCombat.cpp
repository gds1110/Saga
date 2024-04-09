// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_GetUseCombat.h"
#include "AI/SAAI.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CharacterBase.h"
#include "Grid/Grid.h"
#include "Kismet/GameplayStatics.h"
#include <SagaGameMode.h>

UBTTask_GetUseCombat::UBTTask_GetUseCombat()
{
	NodeName = TEXT("GetCombat");

}

EBTNodeResult::Type UBTTask_GetUseCombat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	auto ControllingPawn = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed; // 예외처리. 	


	AGrid* TargetGrid = Cast<AGrid>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGETGRID));
	if (TargetGrid == nullptr)
	{
		if (OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET) != nullptr)
		{
			ACharacterBase* TargetCharacter = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
			if (TargetCharacter != nullptr)
			{
				TargetGrid = TargetCharacter->GetCharacterGrid();
			}
			else
			{
				return EBTNodeResult::Failed;
			}
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}
	TArray<int32> Spells = ControllingPawn->GetCalculateCanAttackSpell(TargetGrid);
	if (Spells.Num()<= 0)
	{
		return EBTNodeResult::Failed;
	}
	int32 UsedSpell = -1;
	float Damage = 0;
	for (int i = 0; i < Spells.Num(); i++)
	{
		if (ControllingPawn->SpellDatas[Spells[i]].D_Damage >= Damage)
		{
			Damage = ControllingPawn->SpellDatas[Spells[i]].D_Damage;
			UsedSpell = Spells[i];
		}
	}
	ControllingPawn->ChangeCurrentSpell(ControllingPawn->SpellDatas[UsedSpell]);
	ControllingPawn->SpellGridDeploy();
	UE_LOG(LogTemp,Log,TEXT("Change Spell To : %s"), *ControllingPawn->SpellDatas[UsedSpell].D_CombatName)
	return EBTNodeResult::Succeeded;
}
