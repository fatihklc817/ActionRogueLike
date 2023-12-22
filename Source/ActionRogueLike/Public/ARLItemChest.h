// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "ARLItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARLItemChest : public AActor, public IARLGameplayInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float TargetPitch;
	
	void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere , BlueprintReadWrite)
	UStaticMeshComponent* LidMesh;

	UPROPERTY(ReplicatedUsing="OnRep_LidOpened")
	bool bLidOpened;

	UFUNCTION()
	void OnRep_LidOpened();
	
public:	
	// Sets default values for this actor's properties
	AARLItemChest();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
};
