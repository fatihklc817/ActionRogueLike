// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "ARLBasePickUp.generated.h"

UCLASS(Abstract)
class ACTIONROGUELIKE_API AARLBasePickUp : public AActor, public IARLGameplayInterface
{
	GENERATED_BODY()
protected:

	UPROPERTY(ReplicatedUsing="OnRep_IsActive")
	bool bIsActive;

	UFUNCTION()
	void OnRep_IsActive();
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	float PickupActivateSecond{10};
	
	FTimerHandle TimerHandle_PickupActivate;
	
public:	
	// Sets default values for this actor's properties
	AARLBasePickUp();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	virtual void ReActivatePickup();
	virtual void DisablePickup();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
};
