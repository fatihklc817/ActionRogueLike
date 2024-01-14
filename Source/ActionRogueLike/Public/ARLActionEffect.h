// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLAction.h"
#include "ARLActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARLActionEffect : public UARLAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="effect")
	float Duration;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="effect")
	float Period;

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

public:
	UARLActionEffect();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

	UFUNCTION(BlueprintCallable, Category="Action")
	float GetTimeRemaining() const;
	
protected:
	UFUNCTION(BlueprintNativeEvent,Category="effect")
	void ExecutePeriodicEffect(AActor* Instigator);
};
