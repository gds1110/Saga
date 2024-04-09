// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterSpellComponent.h"
#include "Character/CharacterBase.h"
#include "Data/CharacterCombatData.h"
#include "Effect/EffectManager.h"
#include "Projectile/BaseProjectile.h"
#include "Grid/Grid.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SagaGameMode.h"

// Sets default values for this component's properties
UCharacterSpellComponent::UCharacterSpellComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCharacterSpellComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	

	ASagaGameMode* gameMode = Cast<ASagaGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode)
	{
		EffectManager = gameMode->EffectManager;
	}
} 


// Called every frame
void UCharacterSpellComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UCharacterSpellComponent::DoSpell(TArray<class AGrid*> RangeGrids)
{
	if (CurrentSpell.SpellNiagara != nullptr)
	{
		EffectManager->SpawnEffectsAtLocation(RangeGrids, CurrentSpell);
	}
}

void UCharacterSpellComponent::DoProjectile(TArray<class AGrid*> RangeGrids)
{
	if (CurrentSpell.D_Projectile != nullptr)
	{
		if (OwnerCharacter != nullptr)
		{
			//FVector SpawnLocation = OwnerCharacter->GetProjectileSocketLocation();
		
			FTimerHandle TimerHandle;

			FVector SpawnLocation = OwnerCharacter->SocketLocation;
			UE_LOG(LogTemp, Log, TEXT("x : %f, y : %f, z : %f"), SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);
			UE_LOG(LogTemp, Log, TEXT("x : %f, y : %f, z : %f"), OwnerCharacter->GetActorLocation().X, OwnerCharacter->GetActorLocation().Y, OwnerCharacter->GetActorLocation().Z);
			ThrowProjectile(CurrentSpell, SpawnLocation);
			//GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
			//	[&]()
			//	{
			//		for (AGrid* InGrid : RangeGrids)
			//		{
			//			/*ABaseProjectile* newProjectile = GetWorld()->SpawnActor<ABaseProjectile>(CurrentSpell.D_Projectile);
			//			newProjectile->SetActorLocation(SpawnLocation);*/

			//			//ThrowProjectile(CurrentSpell, SpawnLocation);
			//			ABaseProjectile* newProjectile = GetWorld()->SpawnActor<ABaseProjectile>(CurrentSpell.D_Projectile);
			//			newProjectile->SetActorLocation(SpawnLocation);
			//		}
			//	}
			//	), CurrentSpell.ProjectileTiming, false);
			if (CurrentSpell.SpellNiagara != nullptr)
			{
				EffectManager->SpawnEffectsAtLocation(RangeGrids, CurrentSpell);
			}
			//newProjectile->SetProjectile(InGrid->OnCharacter);
				//GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
			}
			
		}
		//newProjectile->SetProjectile();
		//newBp;
}



void UCharacterSpellComponent::ThrowProjectile(FCharacterCombatData SpellData, FVector SpawnLocation)
{
	ABaseProjectile* newProjectile = GetWorld()->SpawnActor<ABaseProjectile>(SpellData.D_Projectile);
	newProjectile->SetActorLocation(SpawnLocation);
}

void UCharacterSpellComponent::ChangeSpell(FCharacterCombatData InData)
{
	CurrentSpell = InData;
}

