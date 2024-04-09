// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterStat/CharacterStatComponent.h"
#include "UI/MyWidgetComponent.h"
#include "UI/HpBarWidget.h"
#include "UI/CharacterProgressBar.h"
#include "Grid/GridManager.h"
#include "Grid/Grid.h"
#include "AI/SpawnAIController.h"
#include "AIController.h"
#include "AITypes.h"
#include "Engine/DecalActor.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/CharacterSpellComponent.h"
#include "Engine/DamageEvents.h"
#include "SagaGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Turn\TurnManager.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Animation/BaseAnimInstance.h"

// Sets default values
ACharacterBase::ACharacterBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;
	IsMyTurn = false;
	IsMyTurnEnd = false;
	IsAIMode = false;
	//Pawn
	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = true;
	bUseControllerRotationYaw = true;

	//Capsule
	GetCapsuleComponent()->InitCapsuleSize(1.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	//Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 0.f;
	GetCharacterMovement()->AirControl = 0.f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	//Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/sk_CharM_Base.sk_CharM_Base'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Saga/Animation/InfinityWarriors/ABP_InfinityCharacter.ABP_InfinityCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	// Stat Component
	Stat = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("Stat"));
	SpellComponent = CreateDefaultSubobject<UCharacterSpellComponent>(TEXT("SpellCompo"));
	

	////Widget Component
	//HpBar = CreateDefaultSubobject<UMyWidgetComponent>(TEXT("Widget"));
	//HpBar->SetupAttachment(GetMesh());
	//HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	//static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/Saga/UI/WBP_HpBar.WBP_HpBar_C"));
	//if (HpBarWidgetRef.Class)
	//{
	//	HpBar->SetWidgetClass(HpBarWidgetRef.Class);
	//	HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	//	HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
	//	HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//}
	// 
	//Widget Component
	ValueProgressBar = CreateDefaultSubobject<UMyWidgetComponent>(TEXT("ProgressWidget"));
	ValueProgressBar->SetupAttachment(GetMesh());
	ValueProgressBar->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> ValueBarWidgetRef(TEXT("/Game/Saga/UI/WBP_ValueBar.WBP_ValueBar_C"));
	if (ValueBarWidgetRef.Class)
	{
		ValueProgressBar->SetWidgetClass(ValueBarWidgetRef.Class);
		ValueProgressBar->SetWidgetSpace(EWidgetSpace::Screen);
		ValueProgressBar->SetDrawSize(FVector2D(150.0f, 25.0f));
		ValueProgressBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//Weapon Component
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	AIControllerClass = ASpawnAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialRef(TEXT("/Script/Engine.Material'/Game/Saga/Materials/Decal/M_SelectDecalMaterial.M_SelectDecalMaterial'"));
	
	CharacterDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	CharacterDecal->SetupAttachment(RootComponent);
	CharacterDecal->DecalSize = FVector(30.0f, 60.0f, 60.0f);
	CharacterDecal->SetRelativeLocation(FVector(0, 0, -80.0f));
	CharacterDecal->SetRelativeRotation(FRotator(90.0f, 0, 0));
	
	GetMesh()->bReceivesDecals = false;
	if (DecalMaterialRef.Object)
	{
		CharacterDecal->SetMaterial(0, DecalMaterialRef.Object);
	}
	CharacterDecal->SetVisibility(false);

	bIsMoving = false;
	bIsHit = false;
	bIsAttack = false;
	IsDead = false;
	CurrentCommandIndex = 0;
	CurrentAtkSpd = 0;
	CurState = ECharacterState::WAIT;

	EffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EffectCompo"));
	EffectComponent->SetupAttachment(RootComponent);
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MyTurnNumber = -99;
	//FName SocketName = "ProjectileSocket";
	//ProjectileSocket = GetMesh()->GetSocketByName(SocketName);
	//SetActorEnableCollision(false);

}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &ACharacterBase::SetDead);
	FName SocketName = "ProjectileSocket";
	SocketLocation = GetMesh()->GetSocketLocation(SocketName);

	FName HittedSocketName = "Impact";
	HittedSocketLocation = GetMesh()->GetSocketLocation(HittedSocketName);

}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	AIControl = Cast<ASpawnAIController>(GetController());
	//AIControl->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ACharacterBase::TestFunction);

	CurPC = GetWorld()->GetFirstPlayerController();
	if (CurPC)
	{
		CurCameraComp = CurPC->PlayerCameraManager;
	}
	if (SpellComponent)
	{
		SpellComponent->OwnerCharacter = this;
	}
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (ValueProgressBar)
	//{
	//	if (CurCameraComp)
	//	{
	//		FVector CharLo = GetActorLocation();
	//		FVector CamLo = CurCameraComp->GetTransformComponent()->GetComponentLocation();
	//		FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(CharLo, CamLo);
	//		ValueProgressBar->SetWorldRotation(NewRot);
	//		UE_LOG(LogTemp, Log, TEXT("New Rot"));
	//	}

	//}
	//if (CommandDataArray.Num() > 0)
	//{
	//	DoAction();
	//}\

	if (CharacterDecal != nullptr)
	{
		if (IsMyTurn == true)
		{
			CharacterDecal->SetVisibility(true);

		}
		else
		{
			CharacterDecal->SetVisibility(false);

		}
	}
}

void ACharacterBase::SpawnCharacterInit(AGrid* UnderGrid, FCharacterStuructData InData)
{

	Stat->CharacterStatInit(InData);
	SetCharacterGrid(UnderGrid);
	PlayerType = InData.D_Type;
	D_Thumbnail = InData.D_Thumbnail;
	UDataTable* tempTable = InData.SpellDatatable;
	if (tempTable != nullptr)
	{
		TMap<FName, uint8*> RowMap = tempTable->GetRowMap();

		for (const auto& Entry : RowMap)
		{
			FName RowName = Entry.Key;
			UE_LOG(LogTemp, Warning, TEXT("Row name: %s"), *RowName.ToString());
			//FTableRowBase* MyDataRow = tempTable->FindRow<FTableRowBase>(RowName, FString(""));
			FCharacterCombatData TempSpell = *tempTable->FindRow<FCharacterCombatData>(RowName, FString(""));
			SpellDatas.Add(TempSpell);
		}

	}

	ASagaGameMode* CurrentGameMode = Cast<ASagaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	

	if (CurrentGameMode)
	{
		 TurnMgr=CurrentGameMode->TurnManager;
		 if (TurnMgr != nullptr)
		 {
			 UE_LOG(LogTemp, Log, TEXT("I Have TurnMgr"));
		 }
	}

}

FCommandData ACharacterBase::CreateCommandData(const ECommandType Type, class AGrid* TargetGrid) const
{
	
	FVector CommandLocation = TargetGrid->GetGridLocation_Implementation();

	FRotator CommandRotation = TargetGrid->GetActorRotation();

	AGrid* CommandGrid = TargetGrid;

	AActor* CommandTarget = TargetGrid->GetOnCharacter();


	FCommandData InData = FCommandData(CommandLocation, CommandRotation, Type, CommandGrid);

	return InData;
}

FCommandData ACharacterBase::CreateCommandData(const ECommandType Type, AGrid* TargetGrid, FCharacterCombatData SpellData) const
{
	FVector CommandLocation = TargetGrid->GetGridLocation_Implementation();

	FRotator CommandRotation = TargetGrid->GetActorRotation();

	AGrid* CommandGrid = TargetGrid;

	AActor* CommandTarget = TargetGrid->GetOnCharacter();

	FCharacterCombatData CommandSpell = SpellData;

	FCommandData InData = FCommandData(CommandLocation, CommandRotation, Type, CommandGrid, CommandTarget, CommandSpell);

	return InData;
}

TArray<FCommandData> ACharacterBase::CreateCommandDatas(const TArray<class AGrid*> Grids)
{
	TArray<FCommandData> RetDatas;

	for (AGrid* InGrid : Grids)
	{
		FVector CommandLocation = InGrid->GetGridLocation_Implementation();

		FRotator CommandRotation = InGrid->GetActorRotation();

		AGrid* CommandGrid = InGrid;

		AActor* CommandTarget = InGrid->GetOnCharacter();

		FCommandData InData = FCommandData(CommandLocation,CommandRotation,ECommandType::CommandMove,InGrid,CommandTarget);

		RetDatas.Add(InData);
	}


	return RetDatas;
}

TArray<FCommandData> ACharacterBase::CreateCommandDatas(const ECommandType Type, const TArray<class AGrid*> Grids)
{
	TArray<FCommandData> RetDatas;

	for (AGrid* InGrid : Grids)
	{
		FVector CommandLocation = InGrid->GetGridLocation_Implementation();

		FRotator CommandRotation = FRotator::ZeroRotator;

		AActor* CommandTarget = InGrid->GetOnCharacter();

		FCommandData InData = FCommandData(CommandLocation, CommandRotation, Type, InGrid, CommandTarget);

		RetDatas.Add(InData);
	}


	return RetDatas;
}

TArray<FCommandData> ACharacterBase::CreateCommandDatas(const ECommandType Type, const TArray<class AGrid*> Grids, FCharacterCombatData SpellData)
{
	TArray<FCommandData> RetDatas;

	for (AGrid* InGrid : Grids)
	{
		FVector CommandLocation = InGrid->GetGridLocation_Implementation();

		FRotator CommandRotation = FRotator::ZeroRotator;

		AActor* CommandTarget = InGrid->GetOnCharacter();

		FCharacterCombatData CommandSpell = SpellData;

		FCommandData InData = FCommandData(CommandLocation, CommandRotation, Type, InGrid, CommandTarget,SpellData);

		RetDatas.Add(InData);
	}


	return RetDatas;
}

void ACharacterBase::SetAIController(ASpawnAIController* newAIController)
{
	AIControl = newAIController;
}

void ACharacterBase::DestinationReached(const FCommandData InCommandData)
{
	CommandDataArray.Reset();
	AIControl->CommandDataArray = CommandDataArray;
	GridManager->ChangeGridInformation(MyUnderGrid, InCommandData.CommandGrid);
	//GridManager->CharacterGetPathArea(InCommandData.CommandGrid);
	FindCharacterPath();
	UE_LOG(LogTemp, Log, TEXT("My UnderGrid Is Changed"));
	DeployGrids();
}

void ACharacterBase::FollwingPath()
{


}

void ACharacterBase::SetupCharacterWidget(UMyUserWidget* InUserWidget)
{
	//UHpBarWidget* HpBarWidget = Cast<UHpBarWidget>(InUserWidget);
	//if (HpBarWidget)
	//{
	//	HpBarWidget->SetMaxHp(Stat->GetMaxHp());
	//	HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
	//	Stat->OnHpChanged.AddUObject(HpBarWidget, &UHpBarWidget::UpdateHpBar);
	//}
	UCharacterProgressBar* CharProgWidget = Cast<UCharacterProgressBar>(InUserWidget);
	if (CharProgWidget)
	{
		CharProgWidget->SetMaxValue(ECharValue::HP,Stat->GetMaxValue(ECharValue::HP));
		CharProgWidget->SetMaxValue(ECharValue::MP,Stat->GetMaxValue(ECharValue::MP));
		CharProgWidget->SetMaxValue(ECharValue::STAMINA, Stat->GetMaxValue(ECharValue::STAMINA));
		CharProgWidget->UpdateValueBar(ECharValue::HP, Stat->GetValue(ECharValue::HP));
		CharProgWidget->UpdateValueBar(ECharValue::MP, Stat->GetValue(ECharValue::MP));
		CharProgWidget->UpdateValueBar(ECharValue::STAMINA, Stat->GetValue(ECharValue::STAMINA));
		Stat->OnValueChanged.AddUObject(CharProgWidget, &UCharacterProgressBar::UpdateValueBar);
	}
}

void ACharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);


	//TurnOrderManager로 처리
	if (TurnMgr)
	{
		TurnMgr->RemoveAtTurnMgr(this);
	}

	MyUnderGrid->GridState = EGridState::VALID;
	MyUnderGrid->OnCharacter = nullptr;
	MyUnderGrid = nullptr;
	IsDead = true;

	FTimerHandle DeadTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([&]() {Destroy(); }), 5.0f, false);
	//ASagaGameMode* CurrentGameMode = Cast<ASagaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//

	//if (currentgamemode)
	//{
	//	removeatorder(this);
	//}

	//그리드 초기화 등
	//HpBar->SetHiddenInGame(true);
	//ValueProgressBar->SetHiddenInGame(true);
}

void ACharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	if (DeadMontage)
	{
		AnimInstance->Montage_Play(DeadMontage, 1.0f);
	}
}

void ACharacterBase::MoveCharacter()
{
	AIControl->OnReachedDestination.Clear();
	AIControl->OnReachedDestination.AddDynamic(this, &ACharacterBase::DestinationReached);
	CurrentCommandIndex = 0;
	AIControl->CommandDataArray = CommandDataArray;
	bIsMoving = true;
	FinalPath.Reset();
	CharacterMoveArea.Reset();

}

UCharacterStatComponent* ACharacterBase::GetCharacterStat()
{
	if (Stat!=nullptr)
	{
		return Stat;
	}
	else
	{
		return nullptr;
	}
}

UTexture2D* ACharacterBase::GetCharacterThumnail()
{
	if (Stat->D_Thumbnail)
	{
		return Stat->D_Thumbnail;
	}
	return nullptr;
}

void ACharacterBase::MoveToTarget(AGrid* TargetGrid)
{
	//Simplemove
	if (CharacterMoveArea.Contains(TargetGrid))
	{
		AIControl->MoveToLocation(TargetGrid->GetActorLocation());
		
		/*SetCharacterGrid(TargetGrid);
		FindCharacterPath();*/

	}
}

int32 ACharacterBase::GetCharacterStamina()
{
	return Stat->GetValue(ECharValue::STAMINA);
}

void ACharacterBase::SetCharacterGrid(AGrid* UnderGrid)
{
	if (MyUnderGrid == UnderGrid)
	{
		return;
	}

	if (MyUnderGrid!=nullptr&&MyUnderGrid->OnCharacter != nullptr)
	{
		MyUnderGrid->OnCharacter = nullptr;
	}
	if (UnderGrid != nullptr)
	{
		MyUnderGrid = UnderGrid;
		MyUnderGrid->OnCharacter = this;
		FindCharacterPath();
	}
}

AGrid* ACharacterBase::GetCharacterGrid()
{
	return MyUnderGrid;
}

void ACharacterBase::SetGridManager(AGridManager* gm)
{
	GridManager = gm;
}

void ACharacterBase::SetCharacterArea(TArray<class AGrid*> PathArea)
{

	CharacterMoveArea.Empty();

	CharacterMoveArea = PathArea;
}

TArray<class AGrid*> ACharacterBase::GetCharacterArea()
{
	//캐릭터 이동범위 구하기
	return CharacterMoveArea;
}

void ACharacterBase::TestFunction(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsInterrupted())
	{
		UE_LOG(LogTemp, Log, TEXT("Test Function"));
	}
	if (Result.IsSuccess())
	{
		UE_LOG(LogTemp, Log, TEXT("Is Finished"));
	}
}

void ACharacterBase::CommandMove(const FCommandData InCommandData)
{
	if (!AIControl)
	{
		UE_LOG(LogTemp, Log, TEXT("NO AICONTROL"));

		return;
	}
	AIControl->OnReachedDestination.Clear();
	AIControl->OnReachedDestination.AddDynamic(this, &ACharacterBase::DestinationReached);
	AIControl->CommandMove(InCommandData);

}

void ACharacterBase::SetStatValue(ECharValue ValueType, float InValue)
{
	Stat->SetValue(ValueType, InValue);
}

void ACharacterBase::AddStatValue(ECharValue ValueType, float InValue)
{
	Stat->AddValue(ValueType, InValue); //Int관련 수정
	//if (ValueChangeDelegate.IsBound() == true)
	//{
	//	ValueChangeDelegate.Execute();
	//}
}

void ACharacterBase::SetPathToFollow(TArray<AGrid*> Grids)
{
	if (Grids.IsEmpty())
	{
		return;
	}
	FinalPath = Grids;
	CommandDataArray =CreateCommandDatas(Grids);
	UE_LOG(LogTemp, Log, TEXT("Complete Set Path %d"),Grids.Num());

}

void ACharacterBase::DoAction()
{

	// TODO : Turn To Target


	//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//AnimInstance->Montage_Play();
	
}

void ACharacterBase::DoCommand()
{
	AIControl->StartCommandAction();
}

void ACharacterBase::DoSpell(TArray<class AGrid*> RangeGrids)
{

	// TODO : Turn To Target
	/*if (RangeGrids.Num() > 0)
	{
		FVector TargetLocation = RangeGrids[0]->GetActorLocation();
		FRotator CharRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), CharRotator, GetWorld()->GetDeltaSeconds(), 5.0f);

		SetActorRotation(NewRotation);
	}*/
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(CurrentSpell.D_ActionMontage);
	//TODO :: AIController로 이전
	AddCommands(ECommandType::CommandSpell, RangeGrids);
	if (CurrentSpell.bIsProjectile == true)
	{
		SpellComponent->DoProjectile(RangeGrids);
	}
	else
	{
		SpellComponent->DoSpell(RangeGrids);
	}
	// Command -> Complete -> Command -> Complete System
}

void ACharacterBase::ChangeCurrentSpell(FCharacterCombatData InData)
{
	CurrentSpell = InData;
	SpellComponent->ChangeSpell(InData);
	if (CharacterActionArea.Num() > 0)
	{
		for (AGrid* tempgrid : CharacterActionArea)
		{
			tempgrid->bIsCandidateTarget = false;
		}
	}
	CharacterActionArea.Empty();
	GridManager->ClearAllGrids();
	CharacterActionArea=GridManager->GetSpellRangeGrid(this, InData);

}

void ACharacterBase::SpellGridDeploy()
{
	if (CharacterActionArea.Num() > 0)
	{
		for (AGrid* tempgrid : CharacterActionArea)
		{
			if (tempgrid->GridState != EGridState::INVALID)
			{
				tempgrid->ChangeMaterialColor_Implementation(EGridColor::YELLO);

				if (tempgrid->GetOnCharacter() != nullptr)
				{
					if (tempgrid->GetOnCharacter()->PlayerType != PlayerType)
					{
						tempgrid->bIsCandidateTarget = true;
						tempgrid->ChangeMaterialColor_Implementation(EGridColor::RED);
						tempgrid->GridViewOnOff(tempgrid->GetGridIndex_Implementation(), true);
					}
					if (tempgrid->GetOnCharacter()->PlayerType == PlayerType)
					{
						tempgrid->ChangeMaterialColor_Implementation(EGridColor::GREEN);
					}
				}
		
			}
		}
	}
}

void ACharacterBase::AddCommands(ECommandType Type, AGrid* TargetGrid)
{
	if (AIControl != nullptr)
	{
		//AIControl->CommandDataArray.Add(CreateCommandData(Type, TargetGrid));
		AIControl->CommandDataArray.Add(CreateCommandData(Type, TargetGrid));
		//AIControl->CommandDataQueue.Enqueue(CreateCommandData(Type, TargetGrid));
	}
	//AIControl->StartCommandAction();
}

void ACharacterBase::AddCommands(ECommandType Type, AGrid* TargetGrid, FCharacterCombatData SpellData)
{
	if (AIControl != nullptr)
	{
		AIControl->CommandDataArray.Add(CreateCommandData(Type, TargetGrid,SpellData));
	}
}

void ACharacterBase::AddCommands(ECommandType Type, TArray<class AGrid*> TargetGrids)
{
	for (int32 i = 0; i < TargetGrids.Num(); i++)
	{
		//AIControl->CommandDataArray.Add(CreateCommandData(Type, TargetGrids[i]));
		if (Type == ECommandType::CommandSpell || Type == ECommandType::CommandAttack)
		{
			if (TargetGrids[i]->GetOnCharacter() == nullptr)
			{
				continue;
			}
			AddCommands(Type, TargetGrids[i],CurrentSpell);

		}
		else
		{
			AddCommands(Type, TargetGrids[i]);
		}
	}
}

void ACharacterBase::AddCommands(ECommandType Type, TArray<class AGrid*> TargetGrids, FCharacterCombatData SpellData)
{
}

void ACharacterBase::SetFocusTarget(AGrid* TargetGrid)
{
	if (AIControl != nullptr&&TargetGrid!=nullptr)
	{
			AIControl->FocusTarget = TargetGrid;	
	}
}


float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UBaseAnimInstance* AnimInstance = Cast<UBaseAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->IsAttacked();
	//AddStatValue(ECharValue::HP, -DamageAmount);

	FocusTarget(DamageCauser);

	Stat->ApplyDamage(DamageAmount);
	//bIsA = true;
	//SetDead();
	return DamageAmount;
}

void ACharacterBase::DeployGrids()
{
	if (CharacterMoveArea.Num() > 0)
	{
		for (AGrid* tempgrid : CharacterMoveArea)
		{
			tempgrid->bIsPathTarget = true;
			tempgrid->ChangeMaterialColor_Implementation(EGridColor::GREEN);
			tempgrid->GridViewOnOff(tempgrid->GetGridIndex_Implementation(), true);
		}
	}
	CurState = ECharacterState::MOVEREADY;
}

//생성시 이동가능경로 찾는 함수
void ACharacterBase::FindCharacterPath()
{
	if (CharacterMoveArea.Num() > 0)
	{
		CharacterMoveArea.Reset();
	}
	if (GridManager)
	{
		CharacterMoveArea = GridManager->FindCharacterPath(MyUnderGrid);
	}
}

void ACharacterBase::AttackToTarget(AActor* TargetActor, float Damage)
{
	if (TargetActor)
	{
		FDamageEvent DamageEvent;
		TargetActor->TakeDamage(Damage, DamageEvent, GetController(), this);
		
		UE_LOG(LogTemp, Log, TEXT("Attack To : %s"), *TargetActor->GetName());
	}
}

TArray<TObjectPtr<class AGrid>> ACharacterBase::GetActionArea()
{
	return CharacterActionArea;
}

void ACharacterBase::RollDice()
{
	float NowSpeed = Stat->Speed;
	float Offset = Stat->Agi/10.0f;
	float BeforeRet = NowSpeed + Offset;

	CurrentAtkSpd = FMath::RandRange(-3.0f+BeforeRet, 3.0f+BeforeRet);
	UE_LOG(LogTemp, Log, TEXT("Roll Dice My Spd : %f"), CurrentAtkSpd);

}

FVector ACharacterBase::GetHittedSocketLocation()
{
	FName SocketName = "Impact";

	const USkeletalMeshSocket* PSocket = GetMesh()->GetSocketByName(SocketName);
	if (PSocket)
	{
		//SocketLocation = PSocket->GetSocketLocation(GetMesh());
		HittedSocketLocation = GetMesh()->GetSocketLocation(SocketName);
	}
	return HittedSocketLocation;
}

FVector ACharacterBase::GetProjectileSocketLocation()
{
	FName SocketName = "ProjectileSocket";
	//SocketLocation = GetMesh()->GetSocketLocation(SocketName);
	
	const USkeletalMeshSocket* PSocket = GetMesh()->GetSocketByName(SocketName);
	if (PSocket)
	{
		//SocketLocation = PSocket->GetSocketLocation(GetMesh());
		SocketLocation = PSocket->RelativeLocation;
	}
	return SocketLocation;
}

void ACharacterBase::FocusTarget(AActor* InFocusTarget)
{
	//해당액터 바라보기
	if (InFocusTarget)
	{
		FVector Direction = InFocusTarget->GetActorLocation() - GetActorLocation();
		// 방향 벡터를 가져옵니다.
		Direction.Normalize();

		// 방향 벡터를 기준으로 액터를 회전시킵니다.
		//FRotator NewRotation = FRotationMatrix::MakeFromX(Direction).Rotator();

		// Yaw 회전만을 적용하려는 경우, 다음과 같이 Roll과 Pitch를 0으로 설정할 수 있습니다.
		 FRotator NewRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		 NewRotation.Roll = 0.f;
		 NewRotation.Pitch = 0.f;

		// 액터를 새로운 회전값으로 설정합니다.
		SetActorRotation(NewRotation);
	}

}

AGridManager* ACharacterBase::GetGridManager()
{
	return GridManager;
}

bool ACharacterBase::CalculateCanAttackInRange(AGrid* TargetGrid)
{

	for (int i = 0; i < SpellDatas.Num(); i++)
	{
	  TArray<AGrid*>TempGrids = GridManager->GetSpellRangeGrid(this, SpellDatas[i]);
	  if (TempGrids.Contains(TargetGrid))
	  {
		  return true;
	  }
	}

	return false;
}

bool ACharacterBase::CalculateAttack(AGrid* StartGrid, AGrid* TargetGrid)
{

	// 시작그리드에서 타겟그리드까지 공격가능한지
	for (int i = 0; i < SpellDatas.Num(); i++)
	{
		TArray<AGrid*>TempGrids = GridManager->GetSpellRangeGrid(StartGrid, SpellDatas[i]);
		if (TempGrids.Contains(TargetGrid))
		{
			return true;
		}
	}

	
	return false;
}

bool ACharacterBase::CalculateAttack(AGrid* StartGrid, AGrid* TargetGrid, FCharacterCombatData Spell)
{
	// 시작그리드에서 타겟그리드까지 공격가능한지

	TArray<AGrid*>TempGrids = GridManager->GetSpellRangeGrid(StartGrid, Spell);
	if (TempGrids.Contains(TargetGrid))
	{
		return true;
	}
	return false;
}

TArray<int32> ACharacterBase::GetCalculateCanAttackSpell(AGrid* TargetGrid)
{
	TArray<int32> RetArray;
	for (int i = 0; i < SpellDatas.Num(); i++)
	{
		if (Stat->CurrentMp < SpellDatas[i].D_CostMana)
		{
			continue;
		}

		TArray<AGrid*>TempGrids = GridManager->GetSpellRangeGrid(this, SpellDatas[i]);
		if (TempGrids.Contains(TargetGrid))
		{
			RetArray.Add(i);
		}
	}
	return RetArray;
}

void ACharacterBase::TurnEnd()
{
	//TurnMgr->RemoveAtOnlyUI(this);
	IsMyTurn = false;
	IsMyTurnEnd = true;
	TurnMgr->CheckTurnEnd();
}

void ACharacterBase::TurnOn()
{
	IsMyTurn = true;
	Stat->AddValue(ECharValue::MP,5.0f); //Int관련 수정
	Stat->SetValue(ECharValue::STAMINA, Stat->GetMaxValue(ECharValue::STAMINA));
}

void ACharacterBase::DoneAllAction()
{

}




