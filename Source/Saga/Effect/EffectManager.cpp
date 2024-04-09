// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/EffectManager.h"
#include "Effect/EffectActor.h"
#include "Data/CharacterCombatData.h"
#include "Character/CharacterBase.h"
#include "Grid/Grid.h"

// Sets default values
AEffectManager::AEffectManager(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//ExpandPool();
}

// Called when the game starts or when spawned
void AEffectManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEffectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AEffectActor* AEffectManager::GetPoolObject()
{

	if (EffectActorPool.Num()==0)
	{
		ExpandPool();
	}
	AEffectActor* TempActor = EffectActorPool.Pop();
	//OutedEffectPool.Push(TempActor);
	return TempActor;
}

void AEffectManager::ExpandPool()
{
	for (int i = 0; i < ExpandSize; i++)
	{
		AEffectActor* PoolableEffect = GetWorld()->SpawnActor<AEffectActor>(AEffectActor::StaticClass(),
		FVector().ZeroVector, FRotator().ZeroRotator);
		PoolableEffect->SetObjectPool(this);
		PoolableEffect->SetActive(false);
		EffectActorPool.Push(PoolableEffect);
		//EffectActorPool.Add(PoolableEffect);
	}
	PoolSize += ExpandSize;
}

void AEffectManager::ClearPool()
{
	for (AEffectActor* poolob : OutedEffectPool)
	{
		poolob->ReturnSelf();
	}
	OutedEffectPool.Empty();
}

void AEffectManager::ReturnOjbect(AEffectActor* ReturnObject)
{
	EffectActorPool.Push(ReturnObject);
	
}

void AEffectManager::SpawnEffectAtLocation(AGrid* TargetGrid, FCharacterCombatData SpellData)
{
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);

	if (TargetGrid->GetOnCharacter() != nullptr)
	{
		AEffectActor* TempEffect = GetPoolObject();
		TempEffect->ChangeEffect(SpellData);
		//TempEffect->SetActorLocation(TargetGrid->GetGridLocation_Implementation() + FVector(0, 0, 0.1f));
		TempEffect->SetActorLocation(TargetGrid->GetOnCharacter()->GetHittedSocketLocation());
		
		UE_LOG(LogTemp, Log, TEXT("x : %f, y : %f, z : %f"), TargetGrid->GetOnCharacter()->GetHittedSocketLocation().X, TargetGrid->GetOnCharacter()->GetHittedSocketLocation().Y,
			TargetGrid->GetOnCharacter()->GetHittedSocketLocation().Z);
		TempEffect->SetActive(true);
	}
}

void AEffectManager::SpawnEffectsAtLocation(TArray<class AGrid*> TargetGrids, FCharacterCombatData SpellData)
{
	for (AGrid* InGrid : TargetGrids)
	{
		//if (InGrid->GetOnCharacter() != nullptr)
		//{
		//	AEffectActor* TempEffect = GetPoolObject();
		//	TempEffect->ChangeEffect(SpellData);
		//	//TempEffect->SetActorLocation(InGrid->GetGridLocation_Implementation() + FVector(0, 0, 0.1f));
		//	TempEffect->SetActorLocation(InGrid->GetOnCharacter()->GetHittedSocketLocation());
		//	TempEffect->SetEffectLocation(InGrid);
		//	TempEffect->SetActive(true);
		//}
		SpawnEffectAtLocation(InGrid, SpellData);
	}
}

