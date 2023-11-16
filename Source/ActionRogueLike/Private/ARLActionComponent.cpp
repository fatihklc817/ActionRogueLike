// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLActionComponent.h"

#include "ARLAction.h"


UARLActionComponent::UARLActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UARLActionComponent::BeginPlay()
{
	Super::BeginPlay();
	for(TSubclassOf<UARLAction> actionClass :DefaultActions)
	{
		AddAction(GetOwner(), actionClass);
	}
}


void UARLActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString Debugmsg = GetNameSafe(GetOwner()) + ":" + ActiveGameplayTags.ToStringSimple();
	GEngine -> AddOnScreenDebugMessage(-1,0,FColor::White,Debugmsg);
}

void UARLActionComponent::AddAction(AActor* Instigator, TSubclassOf<UARLAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UARLAction* NewAction = NewObject<UARLAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
		if (NewAction -> bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UARLActionComponent::RemoveAction(UARLAction* ActionToRemove)
{
	if (ensure(ActionToRemove && !ActionToRemove->GetIsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);
	
}

bool UARLActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for(UARLAction* action : Actions)
	{
		if (action && action->ActionName == ActionName)
		{
			if (!action->CanStart(Instigator))
			{
				FString FailedMSg = FString::Printf(TEXT("failed to Run %s"),*ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,FailedMSg);
				continue;
			}
			action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool UARLActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for(UARLAction* action : Actions)
	{
		if (action && action->ActionName == ActionName)
		{
			if (action->GetIsRunning())
			{
				action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}

