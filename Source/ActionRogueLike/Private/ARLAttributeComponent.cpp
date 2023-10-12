// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLAttributeComponent.h"

// Sets default values for this component's properties
UARLAttributeComponent::UARLAttributeComponent()
{
	
}

void UARLAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}

bool UARLAttributeComponent::IsAlive() const
{
	return Health > 0 ;
}


bool UARLAttributeComponent::ApplyHealthChange(float delta)
{
	UE_LOG(LogTemp, Warning, TEXT("healtchangeeedd"));			//actual delta can be added (old health - new health )
	Health += delta;
	Health = FMath::Clamp(Health,0.0f,MaxHealth);
	OnHealthChanged.Broadcast(nullptr,this,Health,delta);
	
	return true;
}

float UARLAttributeComponent::GetHealthAndMaxHealth(float& gettedMaxHealth)
{
	gettedMaxHealth = MaxHealth;
	return  Health;
}

