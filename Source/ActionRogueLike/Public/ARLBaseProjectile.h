// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARLBaseProjectile.generated.h"

UCLASS(Abstract)
class ACTIONROGUELIKE_API AARLBaseProjectile : public AActor
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
	class USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(VisibleAnywhere ,  BlueprintReadOnly)
	UParticleSystemComponent* ParticleSystemComp;


public:	
	// Sets default values for this actor's properties
	AARLBaseProjectile();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

};
