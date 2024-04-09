// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SagaWorldSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class SAGA_API USagaWorldSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void CallThisBP();
	
	UPROPERTY(BlueprintReadWrite)
	int32 IntRef = 2;
	
	UPROPERTY()
	TArray<class ACharacterBase*> PlayerCharacters;	
	UPROPERTY()
	TArray<class ACharacterBase*> EnemyCharacters;
	UFUNCTION()
	void AddCharacters(class ACharacterBase* InputCharacter);
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
