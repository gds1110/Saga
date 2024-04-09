// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPooling/ObjectWithPool.h"

// Sets default values
AObjectWithPool::AObjectWithPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObjectWithPool::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObjectWithPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

