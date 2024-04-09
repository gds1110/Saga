// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTD_IsMyTurn.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API UBTD_IsMyTurn : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
public:
	UBTD_IsMyTurn();
	
};
