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
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed; // ����ó��. 	
	auto Target = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET)); 
	if (nullptr == Target) return EBTNodeResult::Failed; // ����ó��. 	
	FVector LookVector = Target->GetActorLocation() - ControllingPawn->GetActorLocation();	
	LookVector.Z = 0.0f; 
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator(); //������ ����� ������ Rotator�� �޾ƿ� �����Ѵ�.	
	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));// �ڽ��� Rotation���� ������ ���� �������ش�.	
	// RInterpTo ���� (���簪, ��ǥ��, ������ ���࿡ �ɸ��� �ð�, ���� �ӵ�) 	
	return EBTNodeResult::Succeeded; //�׽�ũ ������ �˸���. 

}
