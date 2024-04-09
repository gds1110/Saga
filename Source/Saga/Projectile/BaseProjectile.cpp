// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/BaseProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include "TimerManager.h"

// Sets default values
ABaseProjectile::ABaseProjectile(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MainBodyComponent = CreateDefaultSubobject<USphereComponent>(TEXT("MainBody"));
	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PjtComp"));
	ProjectileParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	RootComponent = MainBodyComponent;
	//ProjectileMeshComponent->SetupAttachment(RootComponent);
	
	ProjectileMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ProjectileParticleComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ProjectileMovementComponent->InitialSpeed = 5000;
	ProjectileMovementComponent->MaxSpeed = 5000;

}

void ABaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}

UProjectileMovementComponent* ABaseProjectile::GetPojectileComponent()
{
	return ProjectileMovementComponent;
}

void ABaseProjectile::SetProjectile(AActor* InTarget)
{
	if (InTarget != nullptr)
	{
		Target = InTarget;
		FVector SelfLocation = GetActorLocation();
		FVector TargetLocation = Target->GetActorLocation();
		FVector TargetDirection = UKismetMathLibrary::GetDirectionUnitVector(SelfLocation, TargetLocation);

		ProjectileMovementComponent->SetVelocityInLocalSpace(ProjectileSpeed * TargetDirection);
	}
	DeadProjectile();
}

void ABaseProjectile::CheckHit()
{
	if (Target != nullptr)
	{
		FVector ProjectileToTarget = GetActorLocation() - Target->GetActorLocation();
		float CalDistance = ProjectileToTarget.Size();

		if (CalDistance < 50.0f)
		{
			UE_LOG(LogTemp, Log, TEXT("Hit Projectile"));
		}

	}
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckHit();
}



void ABaseProjectile::DeadProjectile()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), 3.0f, false);
	
}

