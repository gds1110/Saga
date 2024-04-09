// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Enums.generated.h"

/**
 * 
 */
UENUM()
enum class EPlayerType : uint8
{
	PLAYER,
	ENEMY,
	MAX UMETA(Hidden)
};

UENUM()
enum class EGridState : uint8
{
	VALID,
	INVALID,
	FriendlyUnit,
	EnemyUnit,
	NOTUSED,
	MAX UMETA(Hidden)
};

UENUM()
enum class EPathState : uint8
{
	START,
	PATH,
	TARGET,
	OUTSIDE,
	DEFAULT,
	MAX UMETA(Hidden)
};

UENUM()
enum class ESelMaterial : uint8
{
	GRID,
	PATH,
	MAX UMETA(Hidden)
};
UENUM()
enum class EGridColor : uint8
{
	WHITE,
	GREEN,
	RED,
	BLUE,
	YELLO,
	PURPLE,
	ORANGE,
	MAX UMETA(Hidden)
};
UENUM()
enum class ECombatAttackEnum : uint8
{
	MELEE,
	RANGED,
	MAX UMETA(Hidden)
};
UENUM()
enum class ESpellGridRangeTypeEnum : uint8
{
	DIAGONAL,
	SQUARE,
	LINE,
	MAX UMETA(Hidden)
};
UENUM(BlueprintType)
enum class EGridDirection : uint8
{
	North,
	South,
	East,
	West,
	NorthEast,
	NorthWest,
	SouthEast,
	SouthWest
};
UENUM()
enum class ECharValue : uint8
{
	HP,
	MP,
	STAMINA,
	MAX UMETA(Hidden)
};
UENUM()
enum class ECharacterState : uint8
{
	MOVEREADY,/*이동 준비상태*/
	SPELLREADY,/*스펠(공격) 준비상태*/
	WAIT,/*내 턴이 아닌경우 대기상태*/
	MAX UMETA(Hidden)
};
UENUM(BlueprintType)
enum class ECurrentGameMode : uint8
{
	AIMODE,
	PLAYINGMODE,
	SIMULMODE,
	MAX UMETA(Hidden)
};

UCLASS()
class SAGA_API UEnums : public UObject
{
	GENERATED_BODY()
	
};

