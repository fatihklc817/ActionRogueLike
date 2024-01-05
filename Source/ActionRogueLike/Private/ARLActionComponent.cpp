// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLActionComponent.h"

#include "ARLAction.h"
#include "ActionRogueLike/ActionRogueLike.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"


UARLActionComponent::UARLActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UARLActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator,ActionName);
}


void UARLActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//server only
	if (GetOwner()->HasAuthority())
	{
		for(TSubclassOf<UARLAction> actionClass :DefaultActions)
		{
			AddAction(GetOwner(), actionClass);
		}
	}
}


void UARLActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString Debugmsg = GetNameSafe(GetOwner()) + ":" + ActiveGameplayTags.ToStringSimple();
	//GEngine -> AddOnScreenDebugMessage(-1,0,FColor::White,Debugmsg);

	for (UARLAction* Action : Actions)
	{
		FColor TextColor = Action->GetIsRunning() ? FColor::Blue : FColor::White;
		FString ActionMsg = FString::Printf(TEXT("[%s] Action : %s : Is Running : %s : outer : %s"),
		*GetNameSafe(GetOwner()),
		*Action->ActionName.ToString(),
		Action->GetIsRunning() ? TEXT("true") : TEXT("false"),
		*GetNameSafe(Action->GetOuter())
		);
		
		LogOnScreen(this,ActionMsg,TextColor,0);
	}
}

void UARLActionComponent::AddAction(AActor* Instigator, TSubclassOf<UARLAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UARLAction* NewAction = NewObject<UARLAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
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

			//Is Client
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator,ActionName);
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

void UARLActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARLActionComponent,Actions);
}

bool UARLActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UARLAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action,*Bunch,*RepFlags);
		}
	}
	return WroteSomething;
}

