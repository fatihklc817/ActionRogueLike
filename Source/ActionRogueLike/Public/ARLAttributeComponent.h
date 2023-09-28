// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARLAttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UARLAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Attiributes")
	float Health;

	//healthmax, stamina,strenth

public:	
	// Sets default values for this component's properties
	UARLAttributeComponent();
	

	UFUNCTION(BlueprintCallable,Category="Attiribute")
	bool ApplyHealthChange(float delta);

	
};
