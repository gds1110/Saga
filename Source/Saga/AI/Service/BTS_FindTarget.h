// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_FindTarget.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API UBTS_FindTarget : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTS_FindTarget();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	class ASagaGameMode* CurGameMode;
};
