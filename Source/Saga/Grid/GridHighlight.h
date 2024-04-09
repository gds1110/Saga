// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ChangeColorInterface.h"
#include "GridHighlight.generated.h"

UENUM()
enum class HighlightState
{
	DEFAULT,
	SELECTED,
	INVAILD,
	VAILD,
	MAX UMETA(Hidden)
};

UCLASS()
class SAGA_API AGridHighlight : public AActor, public IChangeColorInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridHighlight();

	void ChangeHighlightColor(HighlightState state);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ChangeMaterialColor(FLinearColor NewColor);

public:
	FORCEINLINE void SetObjectPool(class ASagaPlayerController* InObjectPool) { PoolController = InObjectPool; }
	void ReturnSelf();
	void SetActive(bool IsActive);
	FORCEINLINE bool IsActive() { return bActive; }

private:
	bool bActive;
	class ASagaPlayerController* PoolController;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	HighlightState HLState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HighlightTile")
	TObjectPtr<class UStaticMeshComponent> HighlightMesh;

	UPROPERTY(EditAnywhere, Category = "HighlightMaterial")
	TObjectPtr<class UMaterialInterface> HighlightMaterial;

public:
	UFUNCTION(BlueprintNativeEvent, Category = "MaterialColor")
	void ChangeColor(FLinearColor NewColor);
};
