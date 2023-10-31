// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLPlayerState.h"

float AARLPlayerState::GetCredits()
{
	return Credits;
}

void AARLPlayerState::AddCredits(float delta)
{
	Credits += delta;									//@fixme : check if < 0   // could return bool 
	OnCreditsChanged.Broadcast(Credits,delta);
	//UE_LOG(LogTemp, Warning, TEXT("Credit added in PLAYERSTATEEEE"));
	
}

void AARLPlayerState::RemoveCredits(float delta)
{
	Credits-= delta;                                 //@fixme : check if < 0 and credits < delta 
	OnCreditsChanged.Broadcast(Credits,delta);
}
