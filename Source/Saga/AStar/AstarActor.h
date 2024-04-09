// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AstarActor.generated.h"

UCLASS()
class SAGA_API AAstarActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAstarActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<TArray<TObjectPtr<class AGrid>>> GridArray;

	UPROPERTY(EditAnywhere, Category = "Grid | Layout")
	int32 GridX;

	UPROPERTY(EditAnywhere, Category = "Grid | Layout")
	int32 GridY;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ASTAR
public:
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

	void PathFinding();
	void OpneListAdd(int32 CheckX, int32 CheckY);
	void InitAstarActor(int32 x, int32 y, TArray<TArray<TObjectPtr<class AGrid>>> Array);
};
