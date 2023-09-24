// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARLMagicProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARLMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARLMagicProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
	class USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(VisibleAnywhere ,  BlueprintReadOnly)
	UParticleSystemComponent* ParticleSystemComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
