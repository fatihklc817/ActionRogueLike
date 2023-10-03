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
	UPROPERTY(EditDefaultsOnly,Category="VFX")
	UParticleSystem* ImpactVFX;
	
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly, Category="Components")
	class USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category="Components")
	class UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(VisibleAnywhere ,  BlueprintReadOnly , Category="Components")
	UParticleSystemComponent* EffectParticleSystemComp;

	UPROPERTY(EditDefaultsOnly,Category="Components")
	UAudioComponent* AudioComp;

	UPROPERTY(EditDefaultsOnly,Category="SFX")
	USoundBase* ImpactSound;


public:	
	// Sets default values for this actor's properties
	AARLBaseProjectile();
	
protected:
	// Called when the game starts or when spawned
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;
	

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void Explode();

	

};
