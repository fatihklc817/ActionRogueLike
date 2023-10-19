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

UARLAttributeComponent* UARLAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		 return Cast<UARLAttributeComponent>(FromActor->GetComponentByClass(UARLAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool UARLAttributeComponent::IsActorAlive(AActor* Actor)
{
	UARLAttributeComponent* AttributeComponent = GetAttributes(Actor);
	if (AttributeComponent)
	{
		return AttributeComponent->IsAlive();
	}
	return false;
}

bool UARLAttributeComponent::IsAlive() const
{
	return Health > 0 ;
}


bool UARLAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float delta)
{
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("healtchangeeedd"));			//actual delta can be added (old health - new health )
	float oldHealth = Health;
	Health += delta;
	Health = FMath::Clamp(Health,0.0f,MaxHealth);
	float actualDelta = Health - oldHealth;
	OnHealthChanged.Broadcast(InstigatorActor,this,Health,actualDelta);
	
	return actualDelta != 0;
}

float UARLAttributeComponent::GetHealthAndMaxHealth(float& gettedMaxHealth)
{
	gettedMaxHealth = MaxHealth;
	return  Health;
}

bool UARLAttributeComponent::Kill(AActor* Instigator)
{
   return ApplyHealthChange(Instigator, -MaxHealth);
}

