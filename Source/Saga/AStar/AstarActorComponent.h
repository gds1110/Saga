// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AstarActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAGA_API UAstarActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAstarActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	TArray<TArray<TObjectPtr<class AGrid>>> GridArray;

	UPROPERTY(EditAnywhere, Category = "Grid | Layout")
	int32 GridX;

	UPROPERTY(EditAnywhere, Category = "Grid | Layout")
	int32 GridY;

	UPROPERTY(EditAnywhere,Category=CurCharacter)
	TObjectPtr<AActor> curCharacter;
private:
	UPROPERTY()
	TObjectPtr<class AGridManager> GridManager;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere, Category = "Astar")
	TArray<TObjectPtr<class AGrid>> OpenList;
	UPROPERTY(EditAnywhere, Category = "Astar")
	TArray<TObjectPtr<class AGrid>> ClosedList;
	UPROPERTY(EditAnywhere, Category = "Astar")
	TArray<TObjectPtr<class AGrid>> FinalNodeList;
	UPROPERTY(EditAnywhere, Category = "Astar")
	TObjectPtr<class AGrid> StartNode;
	UPROPERTY(EditAnywhere, Category = "Astar")
	TObjectPtr<class AGrid> TargetNode;
	UPROPERTY(EditAnywhere, Category = "Astar")
	TObjectPtr<class AGrid> CurNode;


	UPROPERTY(EditAnywhere, Category = "Astar")
	TArray<TObjectPtr<class AGrid>> OpenCostList;
	UPROPERTY(EditAnywhere, Category = "Astar")
	TArray<TObjectPtr<class AGrid>> FinalCostList;


	TArray<class AGrid*> GetPathArea(class ACharacterBase* InputCharacter);
	TArray<class AGrid*> GetPathArea(class AGrid* StartGrid);

	TArray<class AGrid*> GetPathFInd(class AGrid* startNode, class AGrid* targetNode);

	void PathFinding();
	void PathFinding(class AGrid* startNode, class AGrid* targetNode);
	int32 GetPathFindingCost(class AGrid* startNode, class AGrid* targetNode);
	void PathFindingNearCost();
	void PathFindingNearCost(int32 Stamina);
	void PathFindingNearCost(class AGrid* startNode);
	void PathFindingNearCost(class ACharacterBase* InputCharacter);
	void PathFindingInArea();
	void OpneListAdd(int32 CheckX, int32 CheckY);
	void OpneCostListAdd(int32 CheckX, int32 CheckY,int32 Stamina);
	void InitAstarActor(int32 x, int32 y, TArray<TArray<TObjectPtr<class AGrid>>> Array);
	void SetGridManager(AGridManager* gm);
	AGridManager* GetGridManager();

	void FindNearGridByNumber(class AGrid* StartGrid, int32 TargetNumber,TArray<class AGrid*>&OutputGrids);
	void OpenListAddByNumber(int32 CheckX, int32 CheckY,TArray<class AGrid*>& TempGrid);
	void FindGridAreaByNumber(AGrid* StartGrid, int32 TargetNumber, TArray<class AGrid*>& OutputGrids);
	FORCEINLINE void setStartNode(class AGrid* start) { StartNode = start; }
	FORCEINLINE void setTargetNode(class AGrid* target) { TargetNode = target; }
};
