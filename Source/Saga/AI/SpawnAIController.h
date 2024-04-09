// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI/AIData.h"
#include "SpawnAIController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReachdDestinationDelegate, const FCommandData, InCommandData);
/**
 * 
 */
UCLASS()
class SAGA_API ASpawnAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ASpawnAIController(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

	UPROPERTY()
	FReachdDestinationDelegate OnReachedDestination;

	UFUNCTION()
	void RunAI();
	UFUNCTION()
	void StopAI();


	void StartCommandAction();
	void CommandMove(const FCommandData InCommandData);	
	void CommandMove();
	UFUNCTION()
	void CommandAction();
	UFUNCTION()
	void TurnToTarget();
	UFUNCTION()
	void CompleteCommandAction();
	UFUNCTION()
	void SpellToTarget();
protected:
	virtual void OnPossess(APawn* InPawn) override;//어떤 컨트롤러가 빙의할때 발생
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UFUNCTION()
	void MoveToCommand();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Owner")
	TObjectPtr<class ACharacterBase> OwningCharacter;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	class UNavigationSystemV1* NavArea;

public:
	UPROPERTY(VisibleAnywhere)
	int32 CurrentIndex;
	UPROPERTY(VisibleAnywhere)
	bool bIsAction;	
	UPROPERTY(VisibleAnywhere)
	bool bIsMoving;
	UPROPERTY(VisibleAnywhere)
	TArray<FCommandData> CommandDataArray;
	UPROPERTY(VisibleAnywhere)
	FCommandData PreCommandData;
	UPROPERTY(VisibleAnywhere)
	FCommandData CurrentCommandData;
	UPROPERTY(VisibleAnywhere)
	FCommandData NextCommandData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RotateTarget")
	AActor* FocusTarget;
	void TargetFocus(AActor* InFocusTarget);
};
