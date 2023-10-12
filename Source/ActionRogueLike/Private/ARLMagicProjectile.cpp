// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLMagicProjectile.h"

#include "ARLAttributeComponent.h"
#include "ARLCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AARLMagicProjectile::AARLMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ProjectileMovementComp -> InitialSpeed = 2000;

	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&AARLMagicProjectile::OnActorOverlap);
	SphereComp->IgnoreActorWhenMoving(GetInstigator(),true);
}

// Called when the game starts or when spawned
void AARLMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AARLMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult)
{
	 auto gettedInstigator = GetInstigator();
	if (ensure(OtherActor) && OtherActor != gettedInstigator)
	{
		
		UARLAttributeComponent* otherActorAttributeComp = Cast<UARLAttributeComponent>(OtherActor->GetComponentByClass(UARLAttributeComponent::StaticClass()));	
		if (otherActorAttributeComp)
		{
			otherActorAttributeComp->ApplyHealthChange(-DamageAmount);
			Explode();
		}
	}
	
}

// Called every frame
void AARLMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

