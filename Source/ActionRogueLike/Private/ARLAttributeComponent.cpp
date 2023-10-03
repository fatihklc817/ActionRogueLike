// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLAttributeComponent.h"

// Sets default values for this component's properties
UARLAttributeComponent::UARLAttributeComponent()
{
	Health = 100;
}

bool UARLAttributeComponent::IsAlive() const
{
	return Health > 0 ;
}


bool UARLAttributeComponent::ApplyHealthChange(float delta)
{
	Health += delta;
	OnHealthChanged.Broadcast(nullptr,this,Health,delta);
	
	return true;
}

