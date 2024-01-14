// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLActionEffect.h"

#include "ARLActionComponent.h"
#include "GameFramework/GameStateBase.h"

UARLActionEffect::UARLActionEffect()
{
	bAutoStart = true;
}

void UARLActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	if (Duration > 0)
	{		FTimerDelegate delegate;
		delegate.BindUFunction(this,"StopAction",Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle,delegate,Duration,false);
		
	}
	if (Period > 0)
	{
		FTimerDelegate delegate;
		delegate.BindUFunction(this,"ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle,delegate,Period,true);
		
	}
	
}

void UARLActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);		
	}
	
	Super::StopAction_Implementation(Instigator);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	UARLActionComponent* ActionComponent = GetOwningComponent();
	if (ActionComponent)
	{
		ActionComponent->RemoveAction(this);
	}

}

void UARLActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	
}

float UARLActionEffect::GetTimeRemaining() const
{
	AGameStateBase* GameState = GetWorld()->GetGameState<AGameStateBase>();
	if (GameState)
	{
		float EndTime = TimeStarted + Duration;
		return EndTime - GameState->GetServerWorldTimeSeconds();
	}

	return Duration; 
}