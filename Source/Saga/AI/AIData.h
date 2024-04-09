// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Data/CharacterCombatData.h"
#include "AIData.generated.h"

UENUM(BlueprintType)
enum ECommandType
{
	CommandMove,
	CommandMoveAttack,
	CommandTurn,
	CommandSpell,
	CommandAttack,
};

USTRUCT(BlueprintType)
struct FCommandData
{
	GENERATED_BODY()
	
	FCommandData() : Location(FVector::ZeroVector),Rotation(FRotator::ZeroRotator),Type(ECommandType::CommandMove),Target(nullptr),CommandGrid(nullptr)
		,CommandSpellData(FCharacterCombatData()),IsPlay(false){}
	
	FCommandData(const FVector InLocation,const FRotator InRotation,const ECommandType InType, class AGrid* InGrid=nullptr,AActor* InTarget = nullptr,
		struct FCharacterCombatData InSpell = FCharacterCombatData(),bool InBool = false):
		Location(InLocation),Rotation(InRotation),Type(InType),Target(InTarget),CommandGrid(InGrid),CommandSpellData(InSpell),IsPlay(InBool){}	


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Location;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator Rotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<ECommandType> Type;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* Target;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class AGrid* CommandGrid;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	struct FCharacterCombatData CommandSpellData;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsPlay;
};