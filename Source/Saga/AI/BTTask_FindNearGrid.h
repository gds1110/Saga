// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindNearGrid.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API UBTTask_FindNearGrid : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_FindNearGrid();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
