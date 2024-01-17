// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ARLAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARLAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Animation")
	bool bIsStunned;

	UPROPERTY(BlueprintReadOnly,Category="Animation")
	class UARLActionComponent* ActionComp;
	
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
