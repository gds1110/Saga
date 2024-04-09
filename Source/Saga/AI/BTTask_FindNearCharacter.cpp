// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindNearCharacter.h"
#include "AI/SAAI.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CharacterBase.h"
#include "Grid/Grid.h"
#include "Kismet/GameplayStatics.h"
#include <SagaGameMode.h>

UBTTask_FindNearCharacter::UBTTask_FindNearCharacter()
{
}

EBTNodeResult::Type UBTTask_FindNearCharacter::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);


	ACharacterBase* ControllingPawn = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	ASagaGameMode* CurrentGameMode = Cast<ASagaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurrentGameMode) {

		//CurrentGameMode->UpdateCharacterList();
		TArray<ACharacterBase*> AllCharacters = CurrentGameMode->AllCharacterList;
		ACharacterBase* TargetActor = nullptr;
		float ClosestDistanceSquared = TNumericLimits<float>::Max();
		for (ACharacterBase* Char : AllCharacters)
		{
			float DistanceSquared = FVector::DistSquared(Char->GetActorLocation(), ControllingPawn->GetActorLocation());
			if (Char->PlayerType != ControllingPawn->PlayerType)
			{
				if (DistanceSquared < ClosestDistanceSquared)
				{
					ClosestDistanceSquared = DistanceSquared;
					TargetActor = Char;
				}
			}
		}
		if (TargetActor!=nullptr)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, TargetActor);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGETGRID, TargetActor->GetCharacterGrid());
			UE_LOG(LogTemp, Log, TEXT("Find Target"));

			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
