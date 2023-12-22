// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARLInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UARLInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly,Category="ui")
	TSubclassOf<class UARLWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	UARLWorldUserWidget* DefaultWidgetInstance;
	
	UPROPERTY(EditDefaultsOnly,Category="trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly,Category="trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly,Category="trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;
	
public:
	UARLInteractionComponent();
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void PrimaryInteract();

	UFUNCTION(Server,Reliable)
	void ServerInteract(AActor* InFocus);

	void FindBestInteractable();

		
};
