// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Tickable.h"
#include "TurnManager.generated.h"

/**
 * 
 */

UCLASS(Blueprintable, BlueprintType)
class SAGA_API UTurnManager : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	UTurnManager(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	TObjectPtr<class ACharacterBase> CurrentTurnCharacter;
	UPROPERTY()
	class UTurnOrderListUI* TurnOrderListUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<class ACharacterBase>> CharacterList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<class ACharacterBase>> UseTurnOrder;
	UPROPERTY(EditAnywhere)
	int32 CurrentNumber;
	UPROPERTY(EditAnywhere)
	int32 MaxNumber;
	UPROPERTY()
	class ASagaGameMode* MyGameMode;
public:
	UFUNCTION()
	void StartNextTurn(); //다음 (전체적인) 턴이 시작된다
	UFUNCTION()
	void StartTurn();
	UFUNCTION(BlueprintCallable,Category="TurnFunction")
	void CalculateTurnOrder();
	UFUNCTION()
	void UpdateOrderUI();
	UFUNCTION()
	void RemoveAtTurnMgr(class ACharacterBase* TargetCharacter);	
	UFUNCTION()
	void RemoveAtOnlyUI(class ACharacterBase* TargetCharacter);
	UFUNCTION()
	void CheckTurnEnd();
public:
	UFUNCTION(BlueprintCallable, Category = Test)
	void CallTimer();
	void TestTimer();


	/** 사용할 Tick함수 입니다. */
	virtual void Tick(float DeltaTime) override;
	/** Tick을 활성화할지 여부를 반환하는 함수입니다. */
	virtual bool IsTickable() const override;
	/** 에디터에서 Tick을 활성화할지 여부를 반환하는 함수입니다. 일반적으로 true를 반환하여 에디터에서도 객체의 Tick을 활성화할 수 있도록 설정합니다. */
	virtual bool IsTickableInEditor() const override;
	/** 게임이 일시 정지되었을 때 Tick을 계속해서 호출할지 여부를 반환하는 함수입니다. */
	virtual bool IsTickableWhenPaused() const override;
	/** 객체의 고유한 통계 식별자를 반환하는 함수입니다. */
	virtual TStatId GetStatId() const override;
	/** 객체가 존재하는 World를 반환하는 함수입니다. */
	virtual UWorld* GetWorld() const override;

protected:
	/** Tick 함수의 활성화/비활성화를 설정하는 변수입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BaseSkill")
	bool bTickable;

	/** 게임이 일시 정지되었을 때 Tick 함수를 호출할지 나타내는 변수입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BaseSkill")
	bool bTickableWhenPaused;
};
