// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARLAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*,InsitagorActor, UARLAttributeComponent*,OwningComp, float,newHealth, float,Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UARLAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

	
protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Attributes")
	float Health{100};

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Attributes")
	float MaxHealth{100};

	//healthmax, stamina,strenth

public:	
	// Sets default values for this component's properties
	UARLAttributeComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	
	UFUNCTION(BlueprintCallable,Category="Attiribute")
	bool ApplyHealthChange(float delta);

	UFUNCTION()
	float GetHealthAndMaxHealth(float& getttedMaxHealth);

	
};
