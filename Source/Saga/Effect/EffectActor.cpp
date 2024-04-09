// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/EffectActor.h"
#include "Effect/EffectManager.h"
#include "Data/CharacterCombatData.h"
#include "Grid/Grid.h"
#include "TimerManager.h"

// Sets default values
AEffectActor::AEffectActor(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	EffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EffectCompo"));
	EffectComponent->SetupAttachment(RootComponent);

	DisableTimeInSeconds = 3.0f;
}

// Called when the game starts or when spawned
void AEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEffectActor::ReturnSelf()
{
	//if()
	if (PoolController == nullptr) return;
	PoolController->ReturnOjbect(this);
	EffectComponent->SetAsset(nullptr);
	SetActive(false);
}

void AEffectActor::SetActive(bool IsActive)
{
	bActive = IsActive;
	SetActorHiddenInGame(!bActive);
	if (IsActive == true)
	{
		GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &AEffectActor::ReturnSelf, DisableTimeInSeconds, false);
	}
}

void AEffectActor::SetEffectLocation(AGrid* TargetGrid)
{
	EffectComponent->SetWorldLocation(TargetGrid->GetGridLocation_Implementation()+ FVector(0, 0, 0.1f));
}

void AEffectActor::SetEffectLocation(FVector TargetLocation)
{
	EffectComponent->SetWorldLocation(TargetLocation);
}

void AEffectActor::ChangeEffect(FCharacterCombatData SpellData)
{
	if (SpellData.SpellNiagara != nullptr)
	{
		EffectComponent->SetAsset(SpellData.SpellNiagara);
	}
}

