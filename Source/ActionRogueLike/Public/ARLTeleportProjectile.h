// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLBaseProjectile.h"
#include "ARLTeleportProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLTeleportProjectile : public AARLBaseProjectile
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly,Category="Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly,Category="Teleport")
	float DetonateDelay;

	FTimerHandle TimerHandle_DelayedDetonate;

public:
	AARLTeleportProjectile();
	
protected:
	virtual void BeginPlay() override;
	
	virtual void Explode_Implementation() override;
	
	void TeleportInstigator();



};
