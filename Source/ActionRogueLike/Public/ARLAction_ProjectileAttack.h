// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLAction.h"
#include "ARLAction_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARLAction_ProjectileAttack : public UARLAction
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere,Category="Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere,Category="Attack") // category was vfx before; 
	FName HandSocketName;

	UPROPERTY(EditDefaultsOnly ,Category="Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere,Category="Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere,Category="Attack")
	UParticleSystem* CastingEffect;

public:
	UARLAction_ProjectileAttack();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;

protected:
	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);
};
