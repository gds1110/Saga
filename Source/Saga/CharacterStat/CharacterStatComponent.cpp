// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatComponent.h"
#include "Data/CharacterStuructData.h"

// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent()
{
	MaxHp = 200.0f;
	CurrentHp = MaxHp;
}

void UCharacterStatComponent::CharacterStatInit(FCharacterStuructData InData)
{

	NowCharacterData = InData;
	D_Thumbnail = InData.D_Thumbnail;
	D_CharacterName = InData.D_CharacterName;
	Level = InData.D_Level;
	MinDamage = InData.D_MinDamage;
	MaxDamage = InData.D_MaxDamage;
	CurrentStamina = InData.D_MaxStamina;
	MaxStamina = InData.D_MaxStamina;
	MaxEnergy = InData.D_MaxEnergy;
	Speed = InData.D_Speed;
	CurrentMp = InData.D_MaxMana;
	MaxMp = InData.D_MaxMana;
	MaxHp = InData.D_MaxHp;
	CurrentHp = InData.D_MaxHp;

	AttackRange = InData.D_AttackRange;

	Agi = InData.D_Agi;
	Str = InData.D_Str;
	Int = InData.D_Int;
}

FCharacterStuructData UCharacterStatComponent::GetCharacterStat()
{

	return NowCharacterData;
}


// Called when the game starts
void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();


	//SetHp(MaxHp);
	SetValue(ECharValue::HP, MaxHp);
	SetValue(ECharValue::MP, MaxMp);
	SetValue(ECharValue::STAMINA, MaxStamina);
}

void UCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	NowCharacterData.D_CurrentHp = NewHp;

	//OnHpChanged.Broadcast(CurrentHp);
	OnValueChanged.Broadcast(ECharValue::HP, CurrentHp);
}

void UCharacterStatComponent::SetValue(ECharValue ValueType, float NewValue)
{
	switch (ValueType)
	{
	case ECharValue::HP:
		CurrentHp = FMath::Clamp<float>(NewValue, 0.0f, MaxHp);
		NowCharacterData.D_CurrentHp = CurrentHp;
		//CurrentHp = NewValue;
		OnValueChanged.Broadcast(ECharValue::HP, CurrentHp);

		//OnHpChanged.Broadcast(CurrentHp);
		break;
	case ECharValue::MP:
		CurrentMp = FMath::Clamp<float>(NewValue, 0.0f, MaxMp);
		NowCharacterData.D_CurrentMana = CurrentMp;
		//CurrentMp = NewValue;
		OnValueChanged.Broadcast(ECharValue::MP, CurrentMp);

		//OnHpChanged.Broadcast(CurrentMp);
		break;
	case ECharValue::STAMINA:
		CurrentStamina = FMath::Clamp<float>(NewValue, 0.0f, MaxStamina);
		NowCharacterData.D_CurrentStamina = CurrentStamina;
		//CurrentStamina = NewValue;
		OnValueChanged.Broadcast(ECharValue::STAMINA, CurrentStamina);

		//OnHpChanged.Broadcast(CurrentStamina);
		break;
	case ECharValue::MAX:
		break;
	default:
		break;
	}
	if (ValueChangeDelegate.IsBound() == true)
	{
		ValueChangeDelegate.Execute();
	}
	
}

float UCharacterStatComponent::GetValue(ECharValue ValueType)
{
	float RetValue = 0;
	switch (ValueType)
	{
	case ECharValue::HP:
		RetValue = CurrentHp;
		break;
	case ECharValue::MP:
		RetValue = CurrentMp;

		break;
	case ECharValue::STAMINA:
		RetValue = CurrentStamina;

		break;
	case ECharValue::MAX:
		break;
	default:
		break;
	}

	return RetValue;
}

float UCharacterStatComponent::GetMaxValue(ECharValue ValueType)
{
	float RetValue = 0;
	switch (ValueType)
	{
	case ECharValue::HP:
		RetValue = MaxHp;
		break;
	case ECharValue::MP:
		RetValue = MaxMp;

		break;
	case ECharValue::STAMINA:
		RetValue = MaxStamina;

		break;
	case ECharValue::MAX:
		break;
	default:
		break;
	}

	return RetValue;
}

void UCharacterStatComponent::AddValue(ECharValue ValueType, float NewValue)
{
	float RetValue = GetValue(ValueType);
	RetValue += NewValue;
	SetValue(ValueType, RetValue);
}

float UCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);
	//SetHp(PrevHp - ActualDamage);
	SetValue(ECharValue::HP, PrevHp - ActualDamage);
	if (CurrentHp <= 0)
	{
		//Á×À½
			if (ValueChangeDelegate.IsBound() == true)
	{
		ValueChangeDelegate.Unbind();
	}
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}




