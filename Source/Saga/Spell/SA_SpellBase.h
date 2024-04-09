// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/CharacterCombatData.h"
#include "Data/Enum/Enums.h"
#include "SA_SpellBase.generated.h"

UCLASS()
class SAGA_API ASA_SpellBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASA_SpellBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCharacterCombatData SpellData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
