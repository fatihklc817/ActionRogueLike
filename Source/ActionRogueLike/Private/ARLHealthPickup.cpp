// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLHealthPickup.h"

#include "ARLAttributeComponent.h"
#include "ARLCharacter.h"


void AARLHealthPickup::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);   // if full health don't pick up -> can be added // but it clamps the health already
	AARLCharacter* InstigatorARLCharacter = Cast<AARLCharacter>(InstigatorPawn);
	InstigatorARLCharacter -> AttributeComponent ->ApplyHealthChange(this,HealthRegenAmount);
}
