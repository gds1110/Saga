// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterSpawn/CharacterSpawnSlot.h"

UCharacterSpawnSlot::UCharacterSpawnSlot(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UCharacterSpawnButton> SpawnButtonClass(TEXT("/Game/Saga/UI/SpawnUI/WBP_SpawnCharacterButton.WBP_SpawnCharacterButton_C"));
	if (SpawnButtonClass.Succeeded())
	{
		SpawnBtnClass = SpawnButtonClass.Class;
		UE_LOG(LogTemp, Log, TEXT("SpawnBtnClass FInd"));

	}

}

void UCharacterSpawnSlot::NativeConstruct()
{
	Super::NativeConstruct();

	
	FName GridName = FName(TEXT("CharacterGridPanel"));
	SpawnCharGrid = Cast<UUniformGridPanel>(GetWidgetFromName(GridName));
	if (SpawnCharGrid != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Find SpawnCharGrid"));
	}
	

}

void UCharacterSpawnSlot::CreateSpawnCharacterButton(FCharacterStuructData data)
{

	if (SpawnCharGrid == NULL)
	{
		FName GridName = FName(TEXT("CharacterGridPanel"));
		SpawnCharGrid = Cast<UUniformGridPanel>(GetWidgetFromName(GridName));
	}
	if (SpawnBtnClass != nullptr)
	{
		UCharacterSpawnButton* ButtonInstance = CreateWidget<UCharacterSpawnButton>(GetWorld(), SpawnBtnClass);
		ButtonInstance->SetSpawnButton(data);
		SpawnCharGrid->AddChildToUniformGrid(ButtonInstance, 0, SpawnCharGrid->GetChildrenCount());
	}
	


}
