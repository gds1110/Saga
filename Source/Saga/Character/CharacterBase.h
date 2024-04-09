// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/CharacterWidgetInterface.h"
#include "Data/CharacterStuructData.h"
#include "AITypes.h"
#include "AI/AIData.h"
#include "NiagaraComponent.h"
#include "CharacterBase.generated.h"



DECLARE_DELEGATE_OneParam(FSpellChangeDelegate, FCharacterCombatData/*스펠 정보*/);


UCLASS()
class SAGA_API ACharacterBase : public ACharacter, public ICharacterWidgetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase(const FObjectInitializer& ObjectInitializer);
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SpawnCharacterInit(class AGrid* UnderGrid, FCharacterStuructData InData);
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpellComponent, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterSpellComponent> SpellComponent;

	UPROPERTY(EditAnywhere,Category=Decal)
	TObjectPtr<class UDecalComponent> CharacterDecal;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AGrid> MyUnderGrid;

	UPROPERTY()
	TArray<FCommandData> CommandDataArray;

	UPROPERTY(EditAnywhere,Category=GridManager)
	TObjectPtr<class AGridManager> GridManager;	
	UPROPERTY(EditAnywhere,Category=PathArea)
	TArray<TObjectPtr<class AGrid>> CharacterMoveArea;
	UPROPERTY(EditAnywhere, Category = PathArea)
	TArray<TObjectPtr<class AGrid>> CharacterActionArea;
	UPROPERTY(EditAnywhere, Category = FollowPath)
	TArray<TObjectPtr<class AGrid>> FinalPath;
	// Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;


	virtual void SetDead();
	void PlayDeadAnimation();

	float DeadEventDelayTime = 5.0f;

	// Command Functions
	UFUNCTION()
	FCommandData CreateCommandData(const ECommandType Type,class AGrid* TargetGrid) const;
	FCommandData CreateCommandData(const ECommandType Type,class AGrid* TargetGrid,FCharacterCombatData SpellData) const;
	TArray<FCommandData> CreateCommandDatas(const TArray<class AGrid*> Grids);
	TArray<FCommandData> CreateCommandDatas(const ECommandType Type,const TArray<class AGrid*> Grids);
	TArray<FCommandData> CreateCommandDatas(const ECommandType Type,const TArray<class AGrid*> Grids, FCharacterCombatData SpellData);
	UFUNCTION()
	void SetAIController(class ASpawnAIController* newAIController);

	// AIMovement Functions
	UFUNCTION()
	void DestinationReached(const FCommandData InCommandData);

	UFUNCTION()
	void FollwingPath();

	// Stat Section
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Stat,Meta= (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterStatComponent> Stat;

	//UI Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMyWidgetComponent> HpBar;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMyWidgetComponent> ValueProgressBar;

	virtual void SetupCharacterWidget(class UMyUserWidget* InUserWidget) override;

	//Item Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Thumnail)
	UTexture2D* D_Thumbnail;


	int32 GetLevel();
	void SetLevel(int32 InNewLevel);
	void MoveCharacter();
	int32 GetCharacterStamina();
	void SetCharacterGrid(class AGrid* UnderGrid);
	class AGrid* GetCharacterGrid();

	void SetGridManager(class AGridManager* gm);

	void SetCharacterArea(TArray<class AGrid*>PathArea);
	TArray<class AGrid*> GetCharacterArea();

	UPROPERTY()
	EPlayerType PlayerType;

	class UCharacterStatComponent* GetCharacterStat();

	class UTexture2D* GetCharacterThumnail();

	void MoveToTarget(class AGrid* TargetGrid);
	void MoveToTargetFollowPath(TArray<FCommandData> InPath);

	TObjectPtr<class ASpawnAIController> AIControl;

	void TestFunction(struct FAIRequestID RequestID,const struct FPathFollowingResult& Result);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AGrid> TargetMoveGrid;

	UPROPERTY(VisibleAnywhere)
	bool bIsMoving;
	UPROPERTY(VisibleAnywhere)
	bool bIsHit;
	UPROPERTY(VisibleAnywhere)
	bool bIsAttack;
	//Command FUNCTINON

	void SetPathToFollow(TArray<AGrid*> Grids);

	UFUNCTION()
	void CommandMove(const FCommandData InCommandData);

	int CurrentCommandIndex;


	void SetStatValue(ECharValue ValueType, float InValue);
	void AddStatValue(ECharValue ValueType, float InValue);

	UPROPERTY()
	TObjectPtr<APlayerController> CurPC;
	UPROPERTY()
	TObjectPtr<class APlayerCameraManager> CurCameraComp;
public:
	UFUNCTION()
	void DoAction();
	UFUNCTION()
	void DoCommand();
	UFUNCTION()
	void DoSpell(TArray<class AGrid*> RangeGrids);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spell)
	FCharacterCombatData CurrentSpell;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spell)
	TArray<FCharacterCombatData> SpellDatas;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	ECharacterState CurState;
	UFUNCTION()
	void ChangeCurrentSpell(FCharacterCombatData InData);
	UFUNCTION()
	void SpellGridDeploy();

	void AddCommands(ECommandType Type,class AGrid* TargetGrid);
	void AddCommands(ECommandType Type,class AGrid* TargetGrid, FCharacterCombatData SpellData);
	void AddCommands(ECommandType Type,TArray<class AGrid*> TargetGrids);
	void AddCommands(ECommandType Type,TArray<class AGrid*> TargetGrids, FCharacterCombatData SpellData);
	void StartCommand();
	FSpellChangeDelegate CharSpellChangeDelegate;

	void SetFocusTarget(class AGrid* TargetGrid);

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Animation)
	TObjectPtr<class UAnimMontage> MeeleActionMontage;

	protected:
		virtual float TakeDamage(float DamageAmount,
			struct FDamageEvent const& DamageEvent,
			class AController* EventInstigator,
			AActor* DamageCauser);


public:
	UFUNCTION(BlueprintCallable)
	void DeployGrids(); //이동가능한 영역 전개
	// 캐릭터의 스태미나 기반으로 AREA 구하기.
	void FindCharacterPath();

	void AttackToTarget(AActor* TargetActor,float Damage);

	TArray<TObjectPtr<class AGrid>> GetActionArea();

	class UTurnManager* TurnMgr;

	class UCharacterOrderSlot* TurnOrderUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsMyTurnEnd;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsMyTurn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAIMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentAtkSpd;
	void RollDice();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SocketLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector HittedSocketLocation;
	FVector GetHittedSocketLocation();
	FVector GetProjectileSocketLocation();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshSocket* ProjectileSocket;
	void FocusTarget(AActor* InFocusTarget);

	class AGridManager* GetGridManager();
	bool CalculateCanAttackInRange(class AGrid* TargetGrid);
	bool CalculateAttack(class AGrid* StartGrid, class AGrid* TargetGrid);
	bool CalculateAttack(class AGrid* StartGrid, class AGrid* TargetGrid,FCharacterCombatData Spell);
	TArray<int32> GetCalculateCanAttackSpell(class AGrid* TargetGrid);
	
	int32 MyTurnNumber;
	//Turn
public:
	void TurnEnd();
	void TurnOn();
	void DoneAllAction();

private:
	TObjectPtr<UNiagaraComponent> EffectComponent;


};
