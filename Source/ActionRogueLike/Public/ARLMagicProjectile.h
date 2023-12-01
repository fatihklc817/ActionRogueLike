// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLBaseProjectile.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "ARLMagicProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARLMagicProjectile : public AARLBaseProjectile
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FGameplayTag ThornTag;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	TSubclassOf<class UARLActionEffect> BurningActionClass;
	
	
public:	
	// Sets default values for this actor's properties
	AARLMagicProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
