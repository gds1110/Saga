// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Data/Enum/Enums.h"
#include "CharacterCombatData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCharacterCombatData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	FCharacterCombatData() : D_CombatName(" "), D_Thumbnail(nullptr), D_CostMana(0), D_RangeMin(0), D_RangeMax(0),
		bIsSingleAction(false), D_SpellGridShapeType(), D_TargetNum(0), D_ActionMontage(nullptr) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString D_CombatName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* D_Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 D_CostMana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 D_RangeMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 D_RangeMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSingleAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESpellGridRangeTypeEnum D_SpellGridShapeType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> D_ActionMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 D_TargetNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsProjectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<class UNiagaraSystem> SpellNiagara;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABaseProjectile> D_Projectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileTiming;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 D_Damage;
};
