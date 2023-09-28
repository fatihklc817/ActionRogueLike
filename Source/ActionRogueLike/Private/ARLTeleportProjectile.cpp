// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLTeleportProjectile.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

void AARLTeleportProjectile::Destroyed()
{
	Super::Destroyed();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),destroyParticle,GetActorLocation());
}

void AARLTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	AActor* Pawn = GetInstigator();
	SphereComp->IgnoreActorWhenMoving(Pawn,true);
}
