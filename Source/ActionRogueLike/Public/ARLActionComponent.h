 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "ARLActionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged, UARLActionComponent*, OwningComp, UARLAction* , Action);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UARLActionComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Tags")
	FGameplayTagContainer ActiveGameplayTags;
	
protected:
	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<class UARLAction*> Actions;

	UPROPERTY(EditAnywhere,Category="Actions")
	TArray<TSubclassOf<UARLAction>> DefaultActions;
	
public:	
	
	UARLActionComponent();

protected:

	UFUNCTION(Server,Reliable)
	void  ServerStartAction(AActor* Instigator, FName ActionName);

	UFUNCTION(Server,Reliable)
	void  ServerStopAction(AActor* Instigator, FName ActionName);
	
	virtual void BeginPlay() override;

	

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Actions")
	void AddAction(AActor* Instigator, TSubclassOf<UARLAction> ActionClass);

	UFUNCTION(BlueprintCallable,Category="Actions")
	void RemoveAction(UARLAction* ActionToRemove);
	
	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StartActionByName(AActor* Instigator,FName ActionName);

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StopActionByName(AActor* Instigator,FName ActionName);

	virtual void GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const override;

	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStarted;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStopped;
	
};
