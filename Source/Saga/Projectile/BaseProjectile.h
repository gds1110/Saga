// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BaseProjectile.generated.h"

UCLASS()
class SAGA_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile(const FObjectInitializer& ObjectInitializer);
	virtual void PostInitializeComponents() override;

	UProjectileMovementComponent* GetPojectileComponent();
	void SetProjectile(AActor* Target);
	void CheckHit();
	void DeadProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (AllowPrivateAceess = "true"))
	TObjectPtr<class USphereComponent> MainBodyComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UStaticMeshComponent> ProjectileMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (AllowPrivateAceess = "true"))
	TObjectPtr<class UStaticMesh> ProjectileMesh;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (AllowPrivateAceess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (AllowPrivateAceess = "true"))
	float ProjectileSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (AllowPrivateAceess = "true"))
	float ProjectileGravity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileEffect", meta = (AllowPrivateAceess = "true"))
	TObjectPtr<class UParticleSystemComponent> ProjectileParticleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileEffect", meta = (AllowPrivateAceess = "true"))
	TObjectPtr<class UParticleSystem> ProjectileBaseParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileEffect", meta = (AllowPrivateAceess = "true"))
	TObjectPtr<class UParticleSystem> ProjectileHitParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (AllowPrivateAceess = "true"))
	float Damage;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	AActor* Target;
	UPROPERTY()
	class ACharacterBase* OwnerCharacter;
	UPROPERTY()
	FVector SpawnSocketLocation;

};
