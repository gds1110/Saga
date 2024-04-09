// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/CharHireLevel.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void ACharHireLevel::BeginPlay()
{

	//APlayerController* playContorller = UGameplayStatics::GetPlayerController(this,0);
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();


	
	
	//playContorller->SetViewTargetWithBlend(playContorller,)

}
