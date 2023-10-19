// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "ARLGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly,Category="ai")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category="ai")
	class UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly,Category="ai")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category="ai")
	UCurveFloat* DifficultyCurve;
	
public:
	AARLGameModeBase();
	
	virtual void StartPlay() override;
	
protected:
	
	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION(Exec)
	void KillAllBots();
};
