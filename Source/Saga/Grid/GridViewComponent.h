// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/ChangeColorInterface.h"
#include "Data/Enum/Enums.h"
#include "GridViewComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAGA_API UGridViewComponent : public UActorComponent, public IChangeColorInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGridViewComponent();

	void InitViewComponent(class AGridManager* gm,class AGrid* ownergrid);

	void BeginPlay() override;
protected:
	UPROPERTY(EditAnywhere, Category = "Grid", meta = (AllowPrivateAceess = "true"))
	TObjectPtr<class UStaticMeshComponent> GridMeshComponent;
	UPROPERTY(EditAnywhere, Category = "Grid", meta = (AllowPrivateAceess = "true"))
	TObjectPtr<class UMaterialInterface> GridMaterialInterface;
	UPROPERTY(EditAnywhere,Category="GridViewMaterials")
	TMap<EGridColor,TObjectPtr<class UMaterialInterface>> GridMaterials;
		
	UPROPERTY(EditAnywhere, Category="OwnerGrid")
	TObjectPtr<class AGrid> OwnerGrid;
	UPROPERTY(EditAnywhere)
	EGridColor CurColor;
public:
	UFUNCTION(BlueprintNativeEvent, Category = "MaterialColor")
	void ChangeMaterialColor(EGridColor color);

	void ChangeGridState(EGridState state);

	void VisualOn();
	void VisualOff();
	UFUNCTION()
	void VisualOnOff(FIntPoint Index,bool OnOff);
};
