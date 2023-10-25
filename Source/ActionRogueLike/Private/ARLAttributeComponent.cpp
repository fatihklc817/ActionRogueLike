// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLAttributeComponent.h"

#include "ARLGameModeBase.h"


static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("arlu.DamageMultiplier"),1,TEXT("Global damage multiplier for attribute comp"),ECVF_Cheat);


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
	if (!GetOwner()->CanBeDamaged() && delta < 0)
	{
		return false;
	}
	
	if (delta < 0)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		delta*= DamageMultiplier;
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("healtchangeeedd"));			//actual delta can be added (old health - new health )
	float oldHealth = Health;
	Health += delta;
	Health = FMath::Clamp(Health,0.0f,MaxHealth);
	float actualDelta = Health - oldHealth;
	OnHealthChanged.Broadcast(InstigatorActor,this,Health,actualDelta);

	//died
	if (actualDelta < 0 && Health == 0)
	{
		AARLGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AARLGameModeBase>();
		if (GameMode)
		{
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
	
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

