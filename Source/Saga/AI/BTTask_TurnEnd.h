// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_TurnEnd.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API UBTTask_TurnEnd : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_TurnEnd();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};
