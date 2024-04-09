// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterSpawn/CharacterSpawnButton.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "SagaPlayerController.h"
#include <Kismet/GameplayStatics.h>


UCharacterSpawnButton::UCharacterSpawnButton(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	if (UCharThumnail == NULL)
	{
		UCharThumnail = Cast<UImage>(GetWidgetFromName(TEXT("WCharImg")));
	}
	if (UCharButton == NULL)
	{
		UCharButton = Cast<UButton>(GetWidgetFromName(TEXT("WButton")));
	}
	if (UNameText == NULL)
	{
		UNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("WCharText")));
	}
	
}

void UCharacterSpawnButton::SetSpawnButton(FCharacterStuructData InputData)
{
	Data = InputData;
}

void UCharacterSpawnButton::SetSpawnInfo()
{
	if (PlayerController != nullptr)
	{
		if (Data.D_StaticMesh != nullptr)
		{
			PlayerController->ChangeStructData(Data);
			UE_LOG(LogTemp, Log, TEXT("Change Input Data"));

		}
	}
	if (PlayerController == nullptr)
	{
		PlayerController = Cast<ASagaPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		if (PlayerController != nullptr)
		{
			if (Data.D_StaticMesh != nullptr)
			{
				PlayerController->ChangeStructData(Data);
				UE_LOG(LogTemp, Log, TEXT("Change Input Data"));

			}
		}
	}

}

void UCharacterSpawnButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayerController == NULL)
	{
		PlayerController = Cast<ASagaPlayerController>(UGameplayStatics::GetPlayerController(this,0));
	}
	

	if (UCharThumnail == NULL)
	{
		UCharThumnail = Cast<UImage>(GetWidgetFromName(TEXT("WCharImg")));
	}
	if (UCharButton == NULL)
	{
		UCharButton = Cast<UButton>(GetWidgetFromName(TEXT("WButton")));
		UCharButton->OnClicked.AddDynamic(this,&UCharacterSpawnButton::SetSpawnInfo);

	}
	if (UNameText == NULL)
	{
		UNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("WCharText")));
	}
	if (Data.D_Thumbnail!=NULL)
	{
		if (UCharThumnail != NULL)
		{
			UCharThumnail->SetBrushFromTexture(Data.D_Thumbnail);
		}
		if (UNameText != NULL)
		{
			UNameText->SetText(FText::FromString(Data.D_CharacterName));
		}
	}
	if (UCharThumnail != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Find UCharThumnail"));
	}

	if (UCharButton != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Find UCharButton"));

	}
	if (UNameText != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Find UNameText"));

	}
	if (UCharThumnail == NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Can't Find UCharThumnail"));
	}

	if (UCharButton == NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Can't Find UCharButton"));

	}
	if (UNameText == NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Can't Find UNameText"));

	}
}
