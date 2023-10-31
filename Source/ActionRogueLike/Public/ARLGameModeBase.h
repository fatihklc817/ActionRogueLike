// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryManager.h"
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

	UPROPERTY(EditDefaultsOnly,Category="pickup")
	float DesiredPickupCount;

	UPROPERTY(EditDefaultsOnly,Category="pickup")
	float MinPickupDistance;

	UPROPERTY(EditDefaultsOnly,Category="pickup")
	UEnvQuery* SpawnPickupQuery;
	
	UPROPERTY()
	FEnvQueryRequest SpawnPickupQueryRequest;
	
	
	UPROPERTY(EditDefaultsOnly,Category="ai")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly,Category="pickup")
	TArray<TSubclassOf<AActor>> PickupClasses;
	
	// UPROPERTY(EditDefaultsOnly,Category="pickup")
	// TSubclassOf<AActor> HealthPickupClass;
	//
	// UPROPERTY(EditDefaultsOnly,Category="pickup")
	// TSubclassOf<AActor> CreditCoinPickupClass;
	

	UPROPERTY(EditDefaultsOnly, Category="ai")
	UCurveFloat* DifficultyCurve;


	
public:
	AARLGameModeBase();
	
	virtual void StartPlay() override;

	virtual void OnActorKilled(AActor* VictimActor,AActor* Killer);
	
protected:
	
	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION(Exec)
	void KillAllBots();

	UFUNCTION()
	void RespawnPlayerTimeElapsed(AController* controller);

	UFUNCTION()
	void SpawnPickups();

	//UFUNCTION()
	//void SpawnCrediCoinPickup();
	
	void OnSpawnPickupsQueryCompleted(TSharedPtr<FEnvQueryResult> Result);

	//void OnSpawnCreditCoinPickupQueryCompleted(TSharedPtr<FEnvQueryResult> Result);
	
};
