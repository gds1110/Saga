// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.h"
#include "Data/Enum/Enums.h"
#include "Data/CharacterStuructData.h"
#include "GridManager.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FGridStateDelegate, FIntPoint,EGridState);

DECLARE_MULTICAST_DELEGATE_TwoParams(FGridOnOffDelegate, FIntPoint,bool);

DECLARE_MULTICAST_DELEGATE_TwoParams(FViewOnOffDelegate, FIntPoint,bool);

DECLARE_MULTICAST_DELEGATE_OneParam(FChangeGridColorDelegate, EGridColor);

DECLARE_MULTICAST_DELEGATE(FRefreshGridDelegate);
DECLARE_MULTICAST_DELEGATE(FClearPathDelegate);
UCLASS()
class SAGA_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

protected:
	TArray<TArray<TObjectPtr<class AGrid>>> GridArray;

	UPROPERTY(EditAnywhere, Category = "Grid | Layout")
	TSubclassOf<class AGrid> BaseGrid;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FIntPoint LocationToTile(FVector location);
	FVector TileToGridLocation(int32 Row, int32 Col, bool IsCenter);

	FGridStateDelegate GridStateDelegate; //정해진 index 상태변경.
	FGridOnOffDelegate GridOnOffDelegate; //정해진 index 껏다키기.
	FViewOnOffDelegate GridViewOnOffDelegate; // 정해진 view index 껏다켜기
	FChangeGridColorDelegate GridColorDelegate; //그리드 내부색깔 변경
	FRefreshGridDelegate GridRefreshDelegate; // 그리드 아웃라인 색깔만 새로고침 델리게이트
	FClearPathDelegate GridPathClearDelegate; // 그리드 패스 초기화 델리게이트
public:
	int32* ptrX;
	int32* ptrY;

	void CheckOvelap();

	UPROPERTY(EditAnywhere, Category = "Grid | Layout")
	int32 GridX;

	UPROPERTY(EditAnywhere, Category = "Grid | Layout")
	int32 GridY;

	UPROPERTY(EditAnywhere, Category = "Grid | Layout")
	float TileSize = 100.0f;

	UPROPERTY()
	FVector AABB_BottomLeftCorner;
	UPROPERTY()
	FVector AABB_TopRightCorner;
	UPROPERTY()
	FBox AABB;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GridMesh)
	TObjectPtr<class UStaticMesh> StoredGridMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GridMaterial)
	TObjectPtr<class UMaterialInterface> StoredGridMaterial;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAstarActorComponent> AstarCompo;

	class UAstarActorComponent* GetAstartCompo();

	void GeneratedGrid(int32 NumX, int32 NumY);
	void GeneratedGridArea(int32 NumX, int32 NumY, float XPos, float YPos, float Radius);
	bool IsInsideAABB(FVector Location);
	void StartPathFind();
	void StartPathFind(ACharacterBase* InputCharacter, int32 Stamina);
	void StartPathFind(AGrid* StartGrid);
	void StartPathFind(class ACharacterBase* InputCharacter);

	void CharacterGetPathArea(AGrid* CharacterOnGrid);

	void SpawnOnGrid(AGrid* TargetGrid, FCharacterStuructData InputData); //Character Spawn
	void ShowGridArea(AGrid* TargetGrid);
	void ClearAllGrids(); //Grid Outline Clear To White
	void ClearPathStateGrids(); // 그리드 패스 상태 클리어.
	void ClearSelectedGrids(TArray<AGrid*> InGrids);
	void ChangeGridInformation(AGrid* PreGrid, AGrid* NewGrid);
	void ChangeGridInformation(AGrid* PreGrid, AGrid* NewGrid,class ACharacterBase* InCharacter);

	UFUNCTION()
	void RefreshGrids();

	TArray<class AGrid*> FindCharacterPath(class ACharacterBase* Character);
	TArray<class AGrid*> FindCharacterPath(class AGrid* CharacterUnderGrid);

	bool CheckInvalidGrid(FIntPoint InputXY);
	TArray<class AGrid*> GetSpellRangeGrid(FIntPoint StartPoint,int32 Min,int32 Max,ESpellGridRangeTypeEnum SpellType);
	TArray<class AGrid*> GetSpellRangeGrid(class ACharacterBase* Character,FCharacterCombatData SpellData);
	TArray<class AGrid*> GetSpellRangeGrid(class AGrid* StartGrid,FCharacterCombatData SpellData);
	TArray<class AGrid*> GetDiagonalGrids(FIntPoint StartPoint, int32 Min, int32 Max);
	TArray<class AGrid*> GetSquereGrids(FIntPoint StartPoint, int32 Min, int32 Max);
	TArray<class AGrid*> GetLineGrids(FIntPoint StartPoint, int32 Min, int32 Max);

	FORCEINLINE float GetGridWidth() { return GridX * TileSize; }
	FORCEINLINE float GetGridHeight() { return GridY * TileSize; }
	FORCEINLINE int32 GetGridX() { return GridX; }
	FORCEINLINE int32 GetGridY() { return GridY; }
	FORCEINLINE TArray<TArray<TObjectPtr<class AGrid>>> GetGridArray() { return GridArray; }

public:
	TArray<class AGrid*> FindBFS(class AGrid* StartGrid);
	TArray<class AGrid*> FindBFS(class AGrid* StartGrid,int32 TargetNum);
	TArray<class AGrid*> FindBFS(class AGrid* StartGrid,FCharacterCombatData SpellData);
	TArray<class AGrid*> FindBFS(class AGrid* StartGrid,FCharacterCombatData SpellData,TArray<AGrid*> ActionGrids);
	void GridFindNeighbors(class AGrid* StartGrid);
};
