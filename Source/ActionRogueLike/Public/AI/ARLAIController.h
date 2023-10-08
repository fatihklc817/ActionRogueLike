// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ARLAIController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="Ai")
	class UBehaviorTree* BehaviourTree;

protected:
	virtual void BeginPlay() override;
};
