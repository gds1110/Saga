// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/CharacterStuructData.h"
#include "CharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float/*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnValueChangedDelegate, ECharValue/*Value Type*/, float/*CurrentValue*/);
DECLARE_DELEGATE(FCharacterUIInfoChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAGA_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatComponent();

	UFUNCTION()
	void CharacterStatInit(FCharacterStuructData InData);
	
	FCharacterStuructData GetCharacterStat();
	FCharacterUIInfoChanged ValueChangeDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleInstanceOnly, Category = Thumnail)
	UTexture2D* D_Thumbnail;
	UPROPERTY(VisibleInstanceOnly, Category = Thumnail)
	FString D_CharacterName;

	UPROPERTY(VisibleInstanceOnly,Category=Stat)
	float MaxHp;

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	int32 MaxMp;

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	int32 MaxStamina; //�̵����� ������
		
	UPROPERTY(Transient,VisibleInstanceOnly, Category = Stat)
	float CurrentHp;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	int32 CurrentMp;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	int32 CurrentStamina;


	FCharacterStuructData NowCharacterData;

public:
	void SetHp(float NewHp);
	void SetValue(ECharValue ValueType, float NewValue);
	float GetValue(ECharValue ValueType);
	float GetMaxValue(ECharValue ValueType);
	void AddValue(ECharValue ValueType, float NewValue);

public:
	FOnHpZeroDelegate OnHpZero;  //����� ����
	FOnHpChangedDelegate OnHpChanged; // ü�º�ȭ�� ����
	FOnValueChangedDelegate OnValueChanged; // ���� ��ȭ�� ����

	FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	FORCEINLINE float GetCurrentStamina(){ return CurrentStamina; }
	FORCEINLINE float GetCurrentMana() { return CurrentMp; }
	float ApplyDamage(float InDamage);

	UPROPERTY(EditAnywhere)
	int32 Level;
	UPROPERTY(EditAnywhere)
	int32 MinDamage;
	UPROPERTY(EditAnywhere)
	int32 MaxDamage;
	UPROPERTY(EditAnywhere)
	int32 AttackRange;


	UPROPERTY(EditAnywhere)
	int32 CurrentEnergy;
	UPROPERTY(EditAnywhere)
	int32 MaxEnergy; //�ൿ���� Ƚ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Speed; //���� �켱�� ���ǵ�



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Agi; //��ø�� (���ǵ� �߰�)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Str; //ü��, �⺻���ݷ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Int; //����, ��ų���ݷ�
};
