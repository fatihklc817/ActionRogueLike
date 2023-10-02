// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLTeleportProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


AARLTeleportProjectile::AARLTeleportProjectile()
{
	TeleportDelay = 0.2;
	DetonateDelay = 0.2;

	ProjectileMovementComp->InitialSpeed = 6000;
}

void AARLTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	AActor* Pawn = GetInstigator();
	SphereComp->IgnoreActorWhenMoving(Pawn,true);
	
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate,this,&AARLTeleportProjectile::Explode_Implementation,DetonateDelay);
	
}

void AARLTeleportProjectile::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	UGameplayStatics::SpawnEmitterAtLocation(this,ImpactVFX,GetActorLocation(),GetActorRotation());
	EffectParticleSystemComp->DeactivateSystem();
	ProjectileMovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_delayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_delayedTeleport,this,&AARLTeleportProjectile::TeleportInstigator,TeleportDelay);

	
}

void AARLTeleportProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation(),ActorToTeleport->GetActorRotation(),false,true);
	}
}
