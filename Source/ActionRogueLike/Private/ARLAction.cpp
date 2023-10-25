// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLAction.h"

void UARLAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Running %s"),*GetNameSafe(this));
}

void UARLAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Stopped %s"),*GetNameSafe(this));
}

UWorld* UARLAction::GetWorld() const
{
	// outer is set when creating action with NewObject<T>
	UActorComponent* ActorComp = Cast<UActorComponent>(GetOuter());
	if (ActorComp)
	{
		return ActorComp->GetWorld();
	}
	return nullptr;
}
