// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARLExplosiveBarrel.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARLExplosiveBarrel : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere)
	class URadialForceComponent* RadialForceComp;
	
public:	
	// Sets default values for this actor's properties
	AARLExplosiveBarrel();

	UFUNCTION(BlueprintCallable)
	void FireImpulseTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp,AActor* OtherActor, UPrimitiveComponent* OtherComp,FVector NormalImpulse, const FHitResult &Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
