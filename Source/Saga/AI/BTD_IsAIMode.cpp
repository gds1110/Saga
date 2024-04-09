// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTD_IsAIMode.h"
#include "Character/CharacterBase.h"
#include "AI/SpawnAIController.h"
#include "AI/SAAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTD_IsAIMode::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	ACharacterBase* ControllingPawn = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		UE_LOG(LogTemp, Log, TEXT("Can't Found Owner"));

		return false;
	}
	if (ControllingPawn->IsAIMode == true)
	{
		return true;
	}

	return false;
}

UBTD_IsAIMode::UBTD_IsAIMode()
{
	NodeName = TEXT("IsAIMode");
}
