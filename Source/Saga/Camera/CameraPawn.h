// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

UCLASS()
class SAGA_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACameraPawn();
	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return CamComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* SceneComponent;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CamComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY()
	FVector	TargetLocation;
	UPROPERTY()
	FRotator TargetRotation;
	UPROPERTY()
	float TargetZoom;
	UPROPERTY()
	bool CanRotate;

	UFUNCTION()
	void CameraBounds();

public:
	UFUNCTION()
	void Forward(float AxisValue);
	UFUNCTION()
	void Right(float AxisValue);
	UFUNCTION()
	void Zoom(float AxisValue);
	UFUNCTION()
	void RotateRight();
	UFUNCTION()
	void RotateLeft();
	UFUNCTION()
	void EnableRotate();
	UFUNCTION()
	void DisableRotate();
	UFUNCTION()
	void RotateHorizontal(float AxisValue);
	UFUNCTION()
	void RotateVertical(float AxisValue);
protected:

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Settings|Camera")
	float MoveSpeed = 20.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Camera")
	float RotateSpeed = 2.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Camera")
	float RotatePitchMin =10.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Camera")
	float RotatePitchMax = 80.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Camera")
	float ZoomSpeed = 2.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Camera")
	float MinZoom = 200.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Camera")
	float MaxZoom = 4000.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
