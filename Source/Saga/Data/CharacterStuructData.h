// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Data/Enum/Enums.h"
#include "Data/CharacterCombatData.h"
#include "CharacterStuructData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCharacterStuructData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString D_CharacterName;

	UPROPERTY(EditAnywhere)
	int32 D_Level=1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float D_MaxHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float D_CurrentHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 D_MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 D_CurrentStamina;
	UPROPERTY(EditAnywhere)
	int32 D_MaxMana; //�ൿ���� Ƚ��
	UPROPERTY(EditAnywhere)
	int32 D_CurrentMana;
	UPROPERTY(EditAnywhere)
	int32 D_MinDamage;
	UPROPERTY(EditAnywhere)
	int32 D_MaxDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 D_MaxEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 D_Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 D_AttackRange;



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 D_Agi; //��ø�� (���ǵ� �߰�)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 D_Str; //ü��, �⺻���ݷ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 D_Int; //����, ��ų���ݷ�

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EPlayerType D_Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* D_Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* D_Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* D_StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ACharacterBase> D_CharacterClass;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TArray<class ASA_SpellBase*> D_CharacterSpells;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* SpellDatatable;
};
