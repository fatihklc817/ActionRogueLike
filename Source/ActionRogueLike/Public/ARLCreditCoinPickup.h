// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLBasePickUp.h"
#include "ARLCreditCoinPickup.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLCreditCoinPickup : public AARLBasePickUp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,Category="CoinPickup")
	float GivenCreditsAmount;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
