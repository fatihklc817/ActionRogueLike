// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLBaseProjectile.h"
#include "ARLBlackHoleProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLBlackHoleProjectile : public AARLBaseProjectile
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	class URadialForceComponent* RadialForceComponent;

	

protected:
	AARLBlackHoleProjectile();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
};
