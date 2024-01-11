// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ARLPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreditsChanged, float, newCredits, float, delta );

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere,Category="PState", ReplicatedUsing="OnRep_Credits")
	float Credits;

	UFUNCTION()
	void OnRep_Credits(float OldCredits);

public:
	UFUNCTION(BlueprintCallable)
	float GetCredits();

	UFUNCTION(BlueprintCallable)
	void AddCredits(float delta);

	UFUNCTION(BlueprintCallable)
	void RemoveCredits(float delta);

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(class UARLSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(UARLSaveGame* SaveObject);
	
};
