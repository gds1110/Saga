// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyWidgetComponent.h"
#include "MyUserWidget.h"

void UMyWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UMyUserWidget* MyUserWdiget = Cast<UMyUserWidget>(GetWidget());
	if (MyUserWdiget)
	{
		MyUserWdiget->SetOwningActor(GetOwner());
	}

}
