// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "ARLAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API UARLAction : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,Category="Action")
	FName ActionName;

	UPROPERTY(EditDefaultsOnly,Category="Action")
	bool bAutoStart;

protected:
	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer GrantsTags;
	
	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer BlockedTags;

	bool bIsRunning;
	
public:
	UFUNCTION(BlueprintNativeEvent,Category="Action")
	bool CanStart(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent,Category="Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Action")
	void StopAction(AActor* Instigator);

	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintCallable,Category="Action")
	bool GetIsRunning()const;

protected:
	UFUNCTION(BlueprintCallable,Category="Action")
	class UARLActionComponent* GetOwningComponent() const ; 
};
