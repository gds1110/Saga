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
	void StartNextTurn(); //���� (��ü����) ���� ���۵ȴ�
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


	/** ����� Tick�Լ� �Դϴ�. */
	virtual void Tick(float DeltaTime) override;
	/** Tick�� Ȱ��ȭ���� ���θ� ��ȯ�ϴ� �Լ��Դϴ�. */
	virtual bool IsTickable() const override;
	/** �����Ϳ��� Tick�� Ȱ��ȭ���� ���θ� ��ȯ�ϴ� �Լ��Դϴ�. �Ϲ������� true�� ��ȯ�Ͽ� �����Ϳ����� ��ü�� Tick�� Ȱ��ȭ�� �� �ֵ��� �����մϴ�. */
	virtual bool IsTickableInEditor() const override;
	/** ������ �Ͻ� �����Ǿ��� �� Tick�� ����ؼ� ȣ������ ���θ� ��ȯ�ϴ� �Լ��Դϴ�. */
	virtual bool IsTickableWhenPaused() const override;
	/** ��ü�� ������ ��� �ĺ��ڸ� ��ȯ�ϴ� �Լ��Դϴ�. */
	virtual TStatId GetStatId() const override;
	/** ��ü�� �����ϴ� World�� ��ȯ�ϴ� �Լ��Դϴ�. */
	virtual UWorld* GetWorld() const override;

protected:
	/** Tick �Լ��� Ȱ��ȭ/��Ȱ��ȭ�� �����ϴ� �����Դϴ�. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BaseSkill")
	bool bTickable;

	/** ������ �Ͻ� �����Ǿ��� �� Tick �Լ��� ȣ������ ��Ÿ���� �����Դϴ�. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BaseSkill")
	bool bTickableWhenPaused;
};
