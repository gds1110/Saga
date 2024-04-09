// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"
#include "AI/SAAI.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CharacterBase.h"
#include "Grid/Grid.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory); 
	auto ControllingPawn = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn()); 
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed; // 예외처리. 	
	auto Target = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET)); 
	if (nullptr == Target) return EBTNodeResult::Failed; // 예외처리. 	
	FVector LookVector = Target->GetActorLocation() - ControllingPawn->GetActorLocation();	
	LookVector.Z = 0.0f; 
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator(); //위에서 계산한 벡터의 Rotator를 받아와 저장한다.	
	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));// 자신의 Rotation값을 보간을 통해 설정해준다.	
	// RInterpTo 인자 (현재값, 목표값, 프레임 수행에 걸리는 시간, 보간 속도) 	
	return EBTNodeResult::Succeeded; //테스크 성공을 알린다. 

}
