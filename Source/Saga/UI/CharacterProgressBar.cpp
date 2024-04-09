// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterProgressBar.h"
#include "Components/ProgressBar.h"
#include "Interface/CharacterWidgetInterface.h"


UCharacterProgressBar::UCharacterProgressBar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	HpMaxValue = -1.0f;	
	MpMaxValue = -1.0f;
	StaminaMaxValue = -1.0f;
}

void UCharacterProgressBar::NativeConstruct()
{
	Super::NativeConstruct();
	HpValueProgerssBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpValueBar")));
	ensure(HpValueProgerssBar);
	MpValueProgerssBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbMpValueBar")));
	ensure(MpValueProgerssBar);
	StaminaValueProgerssBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbStaminaValueBar")));
	ensure(StaminaValueProgerssBar);

	//OwningActor
	ICharacterWidgetInterface* CharacterWidget = Cast<ICharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}

}

void UCharacterProgressBar::SetMaxValue(ECharValue Type, float NewMaxValue)
{

	switch (Type)
	{
	case ECharValue::HP:
		HpMaxValue = NewMaxValue;
		break;
	case ECharValue::MP:
		MpMaxValue = NewMaxValue;
		break;
	case ECharValue::STAMINA:
		StaminaMaxValue = NewMaxValue;
		break;
	case ECharValue::MAX:
		break;
	default:
		break;
	}

}

void UCharacterProgressBar::UpdateValueBar(ECharValue Type, float NewCurrentValeu)
{
	switch (Type)
	{
	case ECharValue::HP:

		ensure(HpMaxValue > 0.0f);

		if (HpValueProgerssBar)
		{
			HpValueProgerssBar->SetPercent(NewCurrentValeu / HpMaxValue);
		}
		break;
	case ECharValue::MP:
		ensure(MpMaxValue > 0.0f);

		if (MpValueProgerssBar)
		{
			MpValueProgerssBar->SetPercent(NewCurrentValeu / MpMaxValue);
		}
		break;
	case ECharValue::STAMINA:
		ensure(StaminaMaxValue > 0.0f);

		if (StaminaValueProgerssBar)
		{
			StaminaValueProgerssBar->SetPercent(NewCurrentValeu / StaminaMaxValue);
		}
		break;
	case ECharValue::MAX:
		break;
	default:
		break;
	}


}
