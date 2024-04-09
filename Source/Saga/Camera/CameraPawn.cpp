// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraPawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACameraPawn::ACameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 2000.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	// Create a camera...
	CamComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnCameraCompo"));
	CamComponent->SetupAttachment(CameraBoom);
	//CamComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
	CameraBounds();

	Super::BeginPlay();
	TargetLocation = GetActorLocation();
	TargetZoom = 2000.f;

	const FRotator Rotation = CameraBoom->GetRelativeRotation();
	TargetRotation = FRotator(Rotation.Pitch + -50, Rotation.Yaw, 0.0f);
}

// Called every frame
void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*const FVector InterpolatedLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, MoveSpeed);
	SetActorLocation(InterpolatedLocation);

	*/
	const FRotator InterpolatedRotation = UKismetMathLibrary::RInterpTo(CameraBoom->GetRelativeRotation(), TargetRotation, DeltaTime, RotateSpeed);
	CameraBoom->SetRelativeRotation(InterpolatedRotation);
	const float InterpolatedZoom = UKismetMathLibrary::FInterpTo(CameraBoom->TargetArmLength, TargetZoom, DeltaTime, ZoomSpeed);
	CameraBoom->TargetArmLength = InterpolatedZoom;
}

// Called to bind functionality to input
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACameraPawn::CameraBounds()
{
	float NewPitch = TargetRotation.Pitch;
	if (TargetRotation.Pitch < (RotatePitchMax * -1))
	{
		NewPitch = (RotatePitchMax * -1);
	}
	else if (TargetRotation.Pitch > (RotatePitchMin * -1))
	{
		NewPitch = (RotatePitchMin * -1);
	}
	TargetRotation = FRotator(NewPitch, TargetRotation.Yaw, 0.f);
	TargetLocation = FVector(TargetLocation.X, TargetLocation.Y, 0.f);
}

void ACameraPawn::Forward(float AxisValue)
{
	if (AxisValue == 0.f)
	{
		return;
	}
	TargetLocation = CameraBoom->GetUpVector() * AxisValue * MoveSpeed + TargetLocation;
}

void ACameraPawn::Right(float AxisValue)
{
	if (AxisValue == 0.f)
	{
		return;
	}
	TargetLocation = CameraBoom->GetRightVector() * AxisValue * MoveSpeed + TargetLocation;
}

void ACameraPawn::Zoom(float AxisValue)
{
	if (AxisValue == 0.f)
	{
		return;
	}
	const float Zoom = AxisValue * -100.f;
	TargetZoom = FMath::Clamp(Zoom + TargetZoom, MinZoom, MaxZoom);
}

void ACameraPawn::RotateRight()
{
	TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.0f, -45, 0.0f));
}

void ACameraPawn::RotateLeft()
{
	TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.0f, 45, 0.0f));

}

void ACameraPawn::EnableRotate()
{
	CanRotate = true;
}

void ACameraPawn::DisableRotate()
{
	CanRotate = false;
}

void ACameraPawn::RotateHorizontal(float AxisValue)
{
	if (AxisValue == 0.f)
	{
		return;
	}
	if (CanRotate)
	{
		TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.f, AxisValue, 0.f));
	}
	//AddActorLocalRotation(FRotator(0.f, AxisValue * RotateSpeed, 0.f));
}

void ACameraPawn::RotateVertical(float AxisValue)
{
	if (AxisValue == 0.f)
	{
		return;
	}
	TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.f, 0.f, AxisValue));
}
