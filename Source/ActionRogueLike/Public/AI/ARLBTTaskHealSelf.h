// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ARLBTTaskHealSelf.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARLBTTaskHealSelf : public UBTTaskNode
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere,Category="ai")
	FBlackboardKeySelector IsLowHealthKey;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
