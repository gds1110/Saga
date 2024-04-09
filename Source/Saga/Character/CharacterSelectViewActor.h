// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterSelectViewActor.generated.h"

USTRUCT(Blueprintable)
struct FSelectViewStruct
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> ViewMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimInstance> AnimInstance;
};


UCLASS()
class SAGA_API ACharacterSelectViewActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterSelectViewActor();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<TObjectPtr<AActor>> SelectStruct;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
