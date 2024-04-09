// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/GridActorInterface.h"
#include "Interface/ChangeColorInterface.h"
#include "Data/Enum/Enums.h"
#include "Data/CharacterStuructData.h"
#include "Grid.generated.h"


UCLASS()
class SAGA_API AGrid : public AActor, public IGridActorInterface, public IChangeColorInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridIndex")
	FIntPoint GridIndex;

protected:
	UPROPERTY(EditAnywhere, Category = "Grid", meta = (AllowPrivateAceess = "true"))
	TObjectPtr<class UStaticMeshComponent> GridMeshComponent;
	UPROPERTY(EditAnywhere, Category = "Grid", meta = (AllowPrivateAceess = "true"))
	TObjectPtr<class UMaterialInterface> GridMaterialInterface;
	UPROPERTY(EditAnywhere, Category = "Grid", meta = (AllowPrivateAceess = "true"))
	TObjectPtr<class UStaticMesh> GridMesh;
	UPROPERTY(EditAnywhere, Category = "Grid", meta = (AllowPrivateAceess = "true"))
	TObjectPtr<class AGridManager> OwnerGridManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = View, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UGridViewComponent> GridViewComponent;


	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere, Category = "Astar")
	int Cost; 
	UPROPERTY(EditAnywhere, Category = "Astar")
	int Heuristic; //H
	UPROPERTY(EditAnywhere, Category = "Astar")
	int F;
	UPROPERTY(EditAnywhere, Category = "Astar")
	int G;
	UPROPERTY(EditAnywhere, Category = "Astar")
	TObjectPtr<AGrid> ParentNode;

	UPROPERTY(EditAnywhere, Category = "Astar")
	EGridState GridState;
	UPROPERTY(EditAnywhere, Category = "Astar")
	EPathState PathState;
	
	UPROPERTY(EditAnywhere, Category = "OnGridActor")
	TObjectPtr<AActor> OnActor;
	UPROPERTY(EditAnywhere, Category = "OnGridCharacter")
	TObjectPtr<class ACharacterBase> OnCharacter;
	UPROPERTY(EditAnywhere, Category = "GridNeighbors")
	TMap<EGridDirection,AGrid*> GridNeighborsMap;

	UPROPERTY(EditAnywhere)
	bool bIsInPath;
	UPROPERTY(EditAnywhere)
	bool bIsOnActor;
	UPROPERTY(EditAnywhere)
	bool bIsTarget;
	UPROPERTY(EditAnywhere)
	bool bIsCandidateTarget;
	UPROPERTY(EditAnywhere)
	bool bIsPathTarget;

public:	


	int GetF() { return Cost + Heuristic; }
	EGridState GetTileState();
	void InitMesh(class UStaticMesh* mesh);
	void InitMaterial(class UMaterialInterface* material);
	void InitBoth(class UStaticMesh* mesh, class UMaterialInterface* material);
	void ChangeMaterialColor(ESelMaterial Sel, FLinearColor NewColor);

	void CheckOverlap();

	void ChangeGridPathState(EPathState state);
	void InitMaterialMap();
	void SetGridManager(class AGridManager* Gm); //GridManager setting and add to delegate
	class AGridManager* GetGridManager();
	//astar 그리드 초기화 함수 넣기 cost 버전도?

	UFUNCTION(BlueprintNativeEvent, Category = "MyInterface")
	FIntPoint GetGridIndex();
	UFUNCTION(BlueprintNativeEvent, Category = "MyInterface")
	FVector GetGridLocation();
	UFUNCTION(BlueprintNativeEvent, Category = "MaterialColor")
	void ChangeColor(FLinearColor NewColor);
	UFUNCTION(BlueprintNativeEvent, Category = "MaterialColor")
	void ChangeMaterialColor(EGridColor color);

	UFUNCTION()
	void ChangeGridMeshColor(FLinearColor NewColor);

	void SetGridColor(EGridColor color);
	
	UFUNCTION()
	void RefreshGrid();
	UFUNCTION()
	void GridStateClear();

	UFUNCTION()
	void ChangeGridState(FIntPoint Index, EGridState state);
	UFUNCTION()
	void VisualOnOff(FIntPoint Index, bool OnOff);
	UFUNCTION()
	void GridViewOnOff(FIntPoint Index, bool OnOff);
	UFUNCTION()
	void SetOnCharacter(FCharacterStuructData InputData);
	UFUNCTION()
	void DeployGrid();

	void SetOnActor(AActor* actor);
	AActor* GetOnActor();
	void ResetOnActor();
	

	class ACharacterBase* GetOnCharacter();

	void CheckOnActorOvelap();
};
