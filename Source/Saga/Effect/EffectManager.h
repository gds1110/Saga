// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EffectManager.generated.h"

UCLASS()
class SAGA_API AEffectManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEffectManager(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Object PoolingHighlight
public:
	class AEffectActor* GetPoolObject();
	void ExpandPool();
	void ClearPool();
	void ReturnOjbect(class AEffectActor* ReturnObject);

	void SpawnEffectAtLocation(class AGrid* TargetGrid,struct FCharacterCombatData SpellData);
	void SpawnEffectsAtLocation(TArray<class AGrid*> TargetGrids,struct FCharacterCombatData SpellData);
public:
	TSubclassOf<class AEffectActor> EffectActorClass;
	int32 PoolSize = 0;
	int32 ExpandSize = 20;
private:
	UPROPERTY(VisibleAnywhere, Category = EffectPool)
	TArray<class AEffectActor*> EffectActorPool;
	UPROPERTY(VisibleAnywhere, Category = EffectPool)
	TArray<class AEffectActor*> OutedEffectPool;
};
