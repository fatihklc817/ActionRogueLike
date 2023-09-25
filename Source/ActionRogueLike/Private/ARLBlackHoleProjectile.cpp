// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLBlackHoleProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

AARLBlackHoleProjectile::AARLBlackHoleProjectile()
{
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComponent->SetupAttachment(RootComponent);
	RadialForceComponent->SetAutoActivate(true);
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);
	RadialForceComponent->AddCollisionChannelToAffect(ECC_PhysicsBody);
	
	
	
	
	ProjectileMovementComp->InitialSpeed = 500;
}

void AARLBlackHoleProjectile::BeginPlay()
{
	Super::BeginPlay();

}

void AARLBlackHoleProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//RadialForceComponent->FireImpulse();
	
}


