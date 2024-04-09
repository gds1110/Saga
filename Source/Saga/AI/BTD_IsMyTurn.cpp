// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTD_IsMyTurn.h"
#include "Character/CharacterBase.h"
#include "AI/SpawnAIController.h"
#include "AI/SAAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


bool UBTD_IsMyTurn::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	

	ACharacterBase* ControllingPawn = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		UE_LOG(LogTemp, Log, TEXT("Can't Found Owner"));

		return false;
	}
	
	if (ControllingPawn->IsMyTurn == true)
	{
		
		//ControllingPawn->AIControl->GetBlackboardComponent()->SetValueAsBool(BBKEY_BTURN, true);
		
		return true;
	}
	else
	{
		//ControllingPawn->AIControl->GetBlackboardComponent()->SetValueAsBool(BBKEY_BTURN, false);

		return false;
	}
}

UBTD_IsMyTurn::UBTD_IsMyTurn()
{
	NodeName = TEXT("IsMyTurn");
}
