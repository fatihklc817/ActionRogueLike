// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ARLMonsterData.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARLMonsterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="SpawnInfo")
	TSubclassOf<AActor> MonsterClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="SpawnInfo")
	TArray<TSubclassOf<class UARLAction>> Actions;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="ui")
	UTexture2D* Icon;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	
};
