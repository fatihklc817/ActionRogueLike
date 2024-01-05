// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLAction.h"

#include "ARLActionComponent.h"
#include "ActionRogueLike/ActionRogueLike.h"
#include "Net/UnrealNetwork.h"

void UARLAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Warning, TEXT("Running %s"),*GetNameSafe(this));
	LogOnScreen(this,FString::Printf(TEXT("Started %s"),*ActionName.ToString()),FColor::Green);

	UARLActionComponent* ActionComponent = GetOwningComponent();

	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);
	bIsRunning = true;
}

void UARLAction::StopAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Warning, TEXT("Stopped %s"),*GetNameSafe(this));
	LogOnScreen(this,FString::Printf(TEXT("Stopped %s"),*ActionName.ToString()),FColor::White);
	
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
	
	UARLActionComponent* ActionComponent = GetOwningComponent();
	if (ActionComponent->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	
	return true;
}

void UARLAction::OnRep_IsRunning()
{
	if (bIsRunning)
	{
		StartAction(nullptr);
	}
	else
	{
		StopAction(nullptr);
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
	return bIsRunning;
}

UARLActionComponent* UARLAction::GetOwningComponent() const
{
	return ActionComp;
}

void UARLAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARLAction,bIsRunning);
	DOREPLIFETIME(UARLAction,ActionComp);
}

