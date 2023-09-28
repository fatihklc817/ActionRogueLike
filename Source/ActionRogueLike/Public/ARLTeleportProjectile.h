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
	UPROPERTY(EditAnywhere)
	class UParticleSystem* destroyParticle; 
	
protected:
	virtual void Destroyed() override;

	virtual void BeginPlay() override;
	
};
