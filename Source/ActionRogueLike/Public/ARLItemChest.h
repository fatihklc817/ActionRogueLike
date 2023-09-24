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

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LidMesh;
	
	
public:	
	// Sets default values for this actor's properties
	AARLItemChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
