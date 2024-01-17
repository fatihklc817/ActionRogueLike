// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLHealthPickup.h"

#include "ARLAttributeComponent.h"
#include "ARLCharacter.h"
#include "ARLPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

void AARLHealthPickup::Interact_Implementation(APawn* InstigatorPawn)
{
	UARLAttributeComponent* AttributeComponent = UARLAttributeComponent::GetAttributes(InstigatorPawn);
	if (ensure(AttributeComponent) && !AttributeComponent->IsFullHealth())
	{
		AARLPlayerState* PState = InstigatorPawn->GetPlayerState<AARLPlayerState>();
		if (PState)
		{
			float playerCredits = PState->GetCredits();
			if (playerCredits >= CreditsAmountToActivate)
			{
				PState->RemoveCredits(CreditsAmountToActivate);
				Super::Interact_Implementation(InstigatorPawn);   // if full health , don't pick up -> can be added // but it clamps the health already
				AARLCharacter* InstigatorARLCharacter = Cast<AARLCharacter>(InstigatorPawn);
				InstigatorARLCharacter -> AttributeComponent ->ApplyHealthChange(this,HealthRegenAmount);
			}
		}
		
	}
	
	
}

FText AARLHealthPickup::GetInteractionText_Implementation(APawn* InstigatorPawn)
{
	UARLAttributeComponent* AttributeComponent = UARLAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComponent && AttributeComponent->IsFullHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning","Already at full health !");
	}

	return  FText::Format( LOCTEXT("HealthPotion_InteractMessage","Cost {0} credits. Restroe health!"),CreditsAmountToActivate);
}

#undef LOCTEXT_NAMESPACE