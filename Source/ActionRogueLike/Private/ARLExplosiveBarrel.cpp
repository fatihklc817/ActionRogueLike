// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLExplosiveBarrel.h"

#include "ARLMagicProjectile.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AARLExplosiveBarrel::AARLExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = MeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(RootComponent);

	RadialForceComp->SetAutoActivate(false);
	RadialForceComp->ForceStrength = 750;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);

	

}

void AARLExplosiveBarrel::FireImpulseTrigger()
{
	RadialForceComp->FireImpulse();
}

// Called when the game starts or when spawned
void AARLExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}

//before begin after constructer
void AARLExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	MeshComp->OnComponentHit.AddDynamic(this,&AARLExplosiveBarrel::OnComponentHit);
}

void AARLExplosiveBarrel::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	if ( auto projectileOtherActor = Cast<AARLMagicProjectile>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("hitted by %s"),*projectileOtherActor->GetName());
		RadialForceComp->FireImpulse();
	}
}

// Called every frame
void AARLExplosiveBarrel::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);

	

}

