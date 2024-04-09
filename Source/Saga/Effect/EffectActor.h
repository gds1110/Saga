// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "EffectActor.generated.h"

UCLASS()
class SAGA_API AEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEffectActor(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE void SetObjectPool(class AEffectManager* InObjectPool) { PoolController = InObjectPool; }
	void ReturnSelf();
	void SetActive(bool IsActive);
	void SetEffectLocation(class AGrid* TargetGrid);
	void SetEffectLocation(FVector TargetLocation);
	FORCEINLINE bool IsActive() { return bActive; }

	void ChangeEffect(struct FCharacterCombatData SpellData);
private:
	bool bActive;
	class AEffectManager* PoolController;

	float DisableTimeInSeconds=3.0f;
	FTimerHandle CountdownTimerHandle;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> EffectComponent;
};
