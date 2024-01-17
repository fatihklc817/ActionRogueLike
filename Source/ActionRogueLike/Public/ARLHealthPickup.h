// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLBasePickUp.h"
#include "ARLGameplayInterface.h"
#include "ARLHealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLHealthPickup : public AARLBasePickUp 
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,Category="HealthPot")
	float HealthRegenAmount;

	UPROPERTY(EditAnywhere,Category="HealthPot")
	float CreditsAmountToActivate{50};

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual FText GetInteractionText_Implementation(APawn* InstigatorPawn) override;
};
