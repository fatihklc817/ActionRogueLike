// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLBaseProjectile.h"
#include "MyARLBaseProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AMyARLBaseProjectile : public AARLBaseProjectile
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	
};
