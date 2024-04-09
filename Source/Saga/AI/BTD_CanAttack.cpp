// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTD_CanAttack.h"
#include "Character/CharacterBase.h"
#include "AI/SpawnAIController.h"
#include "AI/SAAI.h"
#include "AIController.h"
#include "Grid/Grid.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTD_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	ACharacterBase* ControllingPawn = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		UE_LOG(LogTemp, Log, TEXT("Can't Found Owner"));

		return false;
	}
	ACharacterBase* TargetActor = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	bool retBool = false;
	if (TargetActor != nullptr)
	{

		AGrid* TargetGrid = Cast<AGrid>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGETGRID));
		if (TargetGrid != nullptr)
		{
			retBool = ControllingPawn->CalculateCanAttackInRange(TargetGrid);
		 if (retBool == true)
			{
			 UE_LOG(LogTemp, Log, TEXT("Can Attack In Range"));
			
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Can't Found TargetGrid"));
		}
	}
	else
	{

	}


	return retBool;
}

UBTD_CanAttack::UBTD_CanAttack()
{

}
