// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "CharacterStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCharacterStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString D_CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float D_MaxHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 D_MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 D_MaxEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 D_Speed; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* D_Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* D_Mesh;

};
