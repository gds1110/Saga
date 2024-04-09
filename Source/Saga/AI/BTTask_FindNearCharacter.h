// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindNearCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API UBTTask_FindNearCharacter : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FindNearCharacter();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
