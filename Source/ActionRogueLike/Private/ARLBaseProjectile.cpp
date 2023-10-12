// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLBaseProjectile.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AARLBaseProjectile::AARLBaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentHit.AddDynamic(this,&AARLBaseProjectile::OnActorHit);
	RootComponent = SphereComp;

	EffectParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComponent");
	EffectParticleSystemComp->SetupAttachment(SphereComp);

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComp");
	ProjectileMovementComp->InitialSpeed = 8000;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bInitialVelocityInLocalSpace = true;
	ProjectileMovementComp->ProjectileGravityScale = 0;
}

void AARLBaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AARLBaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->IgnoreActorWhenMoving(GetInstigator(),true);
}


void AARLBaseProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor != GetInstigator())
	{
		Explode();
		
	}
	
}

void AARLBaseProjectile::Explode_Implementation()
{
	if (ensure(IsValid(this)))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this,ImpactVFX,GetActorLocation(),GetActorRotation());
		UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation(),GetActorRotation());
		UGameplayStatics::PlayWorldCameraShake(GetWorld(),cameraShakeclass,GetActorLocation(),0,5000);
		Destroy();
	}
}



