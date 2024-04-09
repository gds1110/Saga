// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/GameSingleton.h"

DEFINE_LOG_CATEGORY(LogGameSingleton);

UGameSingleton::UGameSingleton()
{
	UE_LOG(LogTemp, Log, TEXT("Singleton"));

}

UGameSingleton& UGameSingleton::Get()
{
	// TODO: insert return statement here

	UGameSingleton* Singleton = CastChecked<UGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UGameSingleton>();
}
