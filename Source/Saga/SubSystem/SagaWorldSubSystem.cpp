// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/SagaWorldSubSystem.h"
#include "Character/CharacterBase.h"

void USagaWorldSubSystem::CallThisBP()
{
	UE_LOG(LogTemp, Warning, TEXT("Calld in Subsystem Function"));

}

void USagaWorldSubSystem::AddCharacters(ACharacterBase* InputCharacter)
{
	InputCharacter->PlayerType == EPlayerType::PLAYER ? PlayerCharacters.Add(InputCharacter) : EnemyCharacters.Add(InputCharacter);
	// UI ¿¬µ¿
}

void USagaWorldSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("Subsystem Test"));
}
