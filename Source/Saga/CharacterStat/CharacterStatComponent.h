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
	int32 MaxStamina; //이동가능 게이지
		
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
	FOnHpZeroDelegate OnHpZero;  //사망시 실행
	FOnHpChangedDelegate OnHpChanged; // 체력변화시 실행
	FOnValueChangedDelegate OnValueChanged; // 벨류 변화시 실행

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
	int32 MaxEnergy; //행동가능 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Speed; //공격 우선권 스피드



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Agi; //민첩성 (스피드 추가)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Str; //체력, 기본공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Int; //마나, 스킬공격력
};
