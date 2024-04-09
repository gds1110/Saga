// Fill out your copyright notice in the Description page of Project Settings.


#include "Spell/SA_SpellBase.h"

// Sets default values
ASA_SpellBase::ASA_SpellBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASA_SpellBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASA_SpellBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

