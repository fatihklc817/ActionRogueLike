// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "ARLGameModeBase.generated.h"


USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FMonsterInfoRow()
	{
		Weight = 1;
		SpawnCost = 5;
		KillReward = 20;
	}
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FPrimaryAssetId MonsterID;
	
	//change to pick this monster
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	//points required by gamemode to spawn this unit
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float SpawnCost;

	//credits awarded to killer of this unit
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float KillReward;
	
};


/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	FString SlotName;
	
	UPROPERTY()
	class UARLSaveGame* CurrentSaveGame;

	UPROPERTY(EditDefaultsOnly,Category="ai")
	class UDataTable* MonsterTable;
	
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
	
	UPROPERTY(EditDefaultsOnly, Category="ai")
	UCurveFloat* DifficultyCurve;


	
public:
	AARLGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
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

	
	void OnSpawnPickupsQueryCompleted(TSharedPtr<FEnvQueryResult> Result);

	UFUNCTION()
	void OnMonsterLoaded(FPrimaryAssetId LoadedID, FVector SpawnLocation);
	
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	UFUNCTION(BlueprintCallable, Category="save")
	void WriteSaveGame();
	
	void LoadSaveGame();
	
};
