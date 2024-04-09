// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/CharacterCombatData.h"
#include "CharacterSpellComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAGA_API UCharacterSpellComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterSpellComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void DoSpell(TArray<class AGrid*> RangeGrids);
	UFUNCTION()
	void DoProjectile(TArray<class AGrid*> RangeGrids);
	UFUNCTION()
	void ThrowProjectile(FCharacterCombatData SpellData,FVector SpawnLocation);
	UFUNCTION()
	void ChangeSpell(struct FCharacterCombatData InData);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACharacterBase* OwnerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spell)
	FCharacterCombatData CurrentSpell;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spell)
	TArray<FCharacterCombatData> SpellDatas;
	UPROPERTY(VisibleAnywhere, Category = EffectManager)
	TObjectPtr<class AEffectManager> EffectManager;
};
