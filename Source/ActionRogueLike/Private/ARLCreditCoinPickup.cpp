// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLCreditCoinPickup.h"

#include "ARLPlayerState.h"
#include "ActionRogueLike/ActionRogueLike.h"

void AARLCreditCoinPickup::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	AARLPlayerState* PState =  InstigatorPawn->GetPlayerState<AARLPlayerState>();
	if (PState)
	{
		LogOnScreen(this,TEXT("girdim coin"),FColor::Green);
		PState->AddCredits(GivenCreditsAmount);
	}
}
