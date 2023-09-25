// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLBaseProjectile.h"

// Sets default values
AARLBaseProjectile::AARLBaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AARLBaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARLBaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

