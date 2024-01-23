// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLMagicProjectile.h"

#include "ARLActionComponent.h"
#include "ARLAttributeComponent.h"
#include "ARLCharacter.h"
#include "ARLGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ARLActionEffect.h"

// Sets default values
AARLMagicProjectile::AARLMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ProjectileMovementComp -> InitialSpeed = 2000;

	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&AARLMagicProjectile::OnActorOverlap);
	SphereComp->IgnoreActorWhenMoving(GetInstigator(),true);

	InitialLifeSpan = 10.0f;
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
		
		UARLActionComponent* ActionComp = Cast<UARLActionComponent>(OtherActor->GetComponentByClass(UARLActionComponent::StaticClass())); //other is character in the scenario 
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			ProjectileMovementComp->Velocity = -ProjectileMovementComp->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
		
		if(UARLGameplayFunctionLibrary::ApplyDirectionalDamage(gettedInstigator,OtherActor,DamageAmount,SweepResult))
		{
			Explode();
			if (ActionComp && HasAuthority())
			{
				ActionComp->AddAction(gettedInstigator, BurningActionClass);
				if (ActionComp->ActiveGameplayTags.HasTag(ThornTag))
				{
					UARLGameplayFunctionLibrary::ApplyDamage(OtherActor,gettedInstigator,DamageAmount / 2);
				}
			}
		}
		
	}
	
}

// Called every frame
void AARLMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// kod testing on the new keyboard hahahah selamun aleyküm agalar hoşgeldiniz
	
}

