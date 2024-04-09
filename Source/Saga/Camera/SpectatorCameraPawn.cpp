// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/SpectatorCameraPawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


ASpectatorCameraPawn::ASpectatorCameraPawn()
{

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CamComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SpectatorCamera"));
	CamComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CamComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}
