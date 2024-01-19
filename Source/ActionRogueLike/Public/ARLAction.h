 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "ARLAction.generated.h"

/**
 * 
 */
 USTRUCT()
 struct FActionRepData
 {
 	GENERATED_BODY()
 	
 public:
 		UPROPERTY()
 		bool bIsRunning;

 		UPROPERTY()
 		AActor* Instigator;
 };

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
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="ui")
	TSoftObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(Replicated)
	class UARLActionComponent* ActionComp;
	
	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer GrantsTags;
	
	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(Replicated)
	float TimeStarted;
	
	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;
	//bool bIsRunning;

		
	
	UFUNCTION()
	void OnRep_RepData();
	
public:

	void Initialize(class UARLActionComponent* NewActionComp);
	
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

	virtual void GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const override;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
};
