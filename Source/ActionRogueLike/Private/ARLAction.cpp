// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLAction.h"

#include "ARLActionComponent.h"

void UARLAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Running %s"),*GetNameSafe(this));

	UARLActionComponent* ActionComponent = GetOwningComponent();

	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);
	bIsRunning = true;
}

void UARLAction::StopAction_Implementation(AActor* Instigator)
{
	ensureAlways(bIsRunning);
	 
	UE_LOG(LogTemp, Warning, TEXT("Stopped %s"),*GetNameSafe(this));
	UARLActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);
	bIsRunning = false;
}

bool UARLAction::CanStart_Implementation(AActor* Instigator)
{
	if (bIsRunning)
	{
		return false;
	}
	
	UARLActionComponent* ActionComp = GetOwningComponent();
	if (ActionComp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	
	return true;
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

bool UARLAction::GetIsRunning() const
{
	return bIsRunning;
}

UARLActionComponent* UARLAction::GetOwningComponent() const
{
	return Cast<UARLActionComponent>(GetOuter());
}
