// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLGameModeBase.h"

#include "ARLAttributeComponent.h"
#include "ARLCharacter.h"
#include "ARLPlayerState.h"
#include "EngineUtils.h"
#include "AI/ARLAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CvarSpawnBots(TEXT("arlu.SpawnBots"),true,TEXT("enable spawning bots via timer"),ECVF_Cheat);

AARLGameModeBase::AARLGameModeBase()
{
	SpawnTimerInterval = 2;
	
}

void AARLGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots,this,&AARLGameModeBase::SpawnBotTimerElapsed,SpawnTimerInterval,true);

	SpawnPickupQueryRequest = FEnvQueryRequest(SpawnPickupQuery,this);

	SpawnPickups();
}


void AARLGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	AARLCharacter* Player = Cast<AARLCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate_Respawn;
		Delegate_Respawn.BindUFunction(this, "RespawnPlayerTimeElapsed", Player->GetController());

		float RespawnDelay = 2;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay,Delegate_Respawn,RespawnDelay,false);
	}

	APawn* PlayerPawn = Cast<APawn>(Killer);
	if (PlayerPawn)
	{
		AARLPlayerState* PState = PlayerPawn->GetPlayerState<AARLPlayerState>();
		if (PState)
		{
			PState->AddCredits(10);
			//UE_LOG(LogTemp, Warning, TEXT("credit added in gamemode"));
		}
	}
		
	
	
	UE_LOG(LogTemp, Warning, TEXT("On Actor Killed : Victim %s, Killer: %s"),*GetNameSafe(VictimActor),*GetNameSafe(Killer));
}


void AARLGameModeBase::SpawnBotTimerElapsed()
{
	if (!CvarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disable via cvar 'CVarSpawnBots'. "));
		return;
	}
	
	
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
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&AARLGameModeBase::OnSpawnBotQueryCompleted);
	}
}

void AARLGameModeBase::OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
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

void AARLGameModeBase::KillAllBots()
{
	for (AARLAICharacter* bot : TActorRange<AARLAICharacter>(GetWorld()))
	{
		UARLAttributeComponent* AiAttributeComp =  bot->AttributeComponent;   // if use static class  = UARLAttributeComponent::GetAttributes(bot); 
		if (ensure(AiAttributeComp) && AiAttributeComp->IsAlive())
		{
			AiAttributeComp->Kill(this); //  @fixme : pass in player for kill credits
		}
	}

}

void AARLGameModeBase::RespawnPlayerTimeElapsed(AController* controller)
{
	if (controller)
	{
		controller->UnPossess();
		RestartPlayer(controller);
	}
}


void AARLGameModeBase::SpawnPickups()
{
	SpawnPickupQueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &AARLGameModeBase::OnSpawnPickupsQueryCompleted);
}


void AARLGameModeBase::OnSpawnPickupsQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
{
	if (!Result->IsSuccessful())
	{
		UE_LOG(LogTemp, Warning, TEXT("spawn pickup eqs failed"));
		return;
	}
	
	TArray<FVector> locations;
    Result->GetAllAsLocations(locations);
	TArray<FVector> usedLocations;

	int32 spawnCounter = 0;
	while ( spawnCounter <DesiredPickupCount && locations.Num() > 0)
	{
		int randIndex = FMath::RandRange(0,locations.Num()-1);
		FVector pickedLocation = locations[randIndex];
		locations.RemoveAt(randIndex); // to not choose again

		bool bValidLocation = true;
		for (FVector OtherLocation : usedLocations)
		{
			float distanceTo = (pickedLocation-OtherLocation).Size();

			if (distanceTo < MinPickupDistance)
			{
				bValidLocation = false;
				break;
			}
			
		}

		if (!bValidLocation)
		{
			continue;
		}

		int32 RandomClassIndex = FMath::RandRange(0,PickupClasses.Num()-1);
		TSubclassOf<AActor> RandomPickUpClass = PickupClasses[RandomClassIndex];
		FVector SpawnLocationFixed = FVector(pickedLocation.X,pickedLocation.Y,75);
		GetWorld()->SpawnActor<AActor>(RandomPickUpClass,SpawnLocationFixed,FRotator::ZeroRotator);
		UE_LOG(LogTemp, Warning, TEXT("RANDOM Healthhh PİCKJUPPP SPAWNEDDDDDDDDDDDDDDDDDDDDDDDD"));
		spawnCounter++;
	}
	
}



// void AARLGameModeBase::SpawnCrediCoinPickup()
// {
// 	SpawnPickupQueryRequest.Execute(EEnvQueryRunMode::AllMatching,this, &AARLGameModeBase::OnSpawnCreditCoinPickupQueryCompleted);
// }


// void AARLGameModeBase::OnSpawnCreditCoinPickupQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
// {
// 	TArray<FVector> locations;
// 	Result->GetAllAsLocations(locations);
// 	FVector randLocation = locations[FMath::RandRange(0,locations.Num()-1)];
// 	FVector SpawnLocationFixed = FVector(randLocation.X,randLocation.Y,75);
// 	GetWorld()->SpawnActor<AActor>(CreditCoinPickupClass,SpawnLocationFixed,FRotator::ZeroRotator);
// 	UE_LOG(LogTemp, Warning, TEXT("RANDOM COİNNNNN  PİCKJUPPP SPAWNEDDDDDDDDDDDDDDDDDDDDDDDD"));
//
// 	//@fixme add distance to other pickups check in the eqs !! 
// }

