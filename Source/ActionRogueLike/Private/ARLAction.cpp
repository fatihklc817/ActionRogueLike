// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLAction.h"

#include "ARLActionComponent.h"
#include "ActionRogueLike/ActionRogueLike.h"
#include "Net/UnrealNetwork.h"

void UARLAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Running %s"),*GetNameSafe(this));
	//LogOnScreen(this,FString::Printf(TEXT("Started %s"),*ActionName.ToString()),FColor::Green);

	UARLActionComponent* ActionComponent = GetOwningComponent();

	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);
	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	//server
	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}

	ActionComponent->OnActionStarted.Broadcast(ActionComponent,this);
}

void UARLAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Stopped %s"),*GetNameSafe(this));
	//LogOnScreen(this,FString::Printf(TEXT("Stopped %s"),*ActionName.ToString()),FColor::White);
	
	UARLActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);
	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	ActionComponent->OnActionStopped.Broadcast(ActionComponent,this);
}

bool UARLAction::CanStart_Implementation(AActor* Instigator)
{
	if (RepData.bIsRunning)
	{
		return false;
	}
	
	UARLActionComponent* ActionComponent = GetOwningComponent();
	if (ActionComponent->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	
	return true;
}

void UARLAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

void UARLAction::Initialize(UARLActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

UWorld* UARLAction::GetWorld() const
{
	// outer is set when creating action with NewObject<T>
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

bool UARLAction::GetIsRunning() const
{
	return RepData.bIsRunning;
}


UARLActionComponent* UARLAction::GetOwningComponent() const
{
	return ActionComp;
}

void UARLAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARLAction,RepData);
	DOREPLIFETIME(UARLAction,ActionComp);
	DOREPLIFETIME(UARLAction,TimeStarted);
}

