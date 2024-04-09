// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/CharacterWidgetInterface.h"

UHpBarWidget::UHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}


void UHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HpProgerssBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgerssBar);

	//OwningActor
	ICharacterWidgetInterface* CharacterWidget = Cast<ICharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UHpBarWidget::UpdateHpBar(float NewCurrentHp)
{

	ensure(MaxHp > 0.0f);
	if (HpProgerssBar)
	{
		HpProgerssBar->SetPercent(NewCurrentHp / MaxHp);
	}
}
