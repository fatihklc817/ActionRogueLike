// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLGameModeBase.h"

#include "ARLAttributeComponent.h"
#include "EngineUtils.h"
#include "AI/ARLAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

AARLGameModeBase::AARLGameModeBase()
{
	SpawnTimerInterval = 2;
	
}

void AARLGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots,this,&AARLGameModeBase::SpawnBotTimerElapsed,SpawnTimerInterval,true);
}

void AARLGameModeBase::SpawnBotTimerElapsed()
{
	int32 NumOfAliveBots = 0;
	for (AARLAICharacter* bot : TActorRange<AARLAICharacter>(GetWorld()))
	{
		UARLAttributeComponent* AiAttributeComp =  bot->AttributeComponent;   // if use static class  = UARLAttributeComponent::GetAttributes(bot); 
		if (ensure(AiAttributeComp) && AiAttributeComp->IsAlive())
		{
			NumOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("found %i bots"),NumOfAliveBots);
	
	float MaxBotCount = 10;
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
		
	}
	
	if (NumOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("can't spawn bot! Reached to maxcount"));
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance  = UEnvQueryManager::RunEQSQuery(this,SpawnBotQuery,this,EEnvQueryRunMode::RandomBest5Pct,nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&AARLGameModeBase::OnQueryCompleted);
	}
}

void AARLGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success )
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnBot EQS Query Failed"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.Num() > 0)
	{
		FVector SpawnLocationFixed = FVector(Locations[0].X,Locations[0].Y,90);
		GetWorld()->SpawnActor<AActor>(MinionClass, SpawnLocationFixed, FRotator::ZeroRotator);
		DrawDebugSphere(GetWorld(), SpawnLocationFixed,25 ,12,FColor::Orange,true);
	}
}


