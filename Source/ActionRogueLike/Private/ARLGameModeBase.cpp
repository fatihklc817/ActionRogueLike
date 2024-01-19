// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLGameModeBase.h"

#include "ARLActionComponent.h"
#include "ARLAttributeComponent.h"
#include "ARLCharacter.h"
#include "ARLGameplayInterface.h"
#include "ARLMonsterData.h"
#include "ARLPlayerState.h"
#include "ARLSaveGame.h"
#include "EngineUtils.h"
#include "ActionRogueLike/ActionRogueLike.h"
#include "AI/ARLAICharacter.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CvarSpawnBots(TEXT("arlu.SpawnBots"),true,TEXT("enable spawning bots via timer"),ECVF_Cheat);

AARLGameModeBase::AARLGameModeBase()
{
	SpawnTimerInterval = 2;

	SlotName = "SaveGame01";
	
}

void AARLGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();	
}

void AARLGameModeBase::StartPlay()
{
	Super::StartPlay();

	LoadSaveGame();     // bu ne kadar doğru bilmiyorum ama iki kere çağırarak hem creditsin hem de actor transformlarının yüklenmesini sağladım

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
		if (MonsterTable)
		{
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("",Rows);

			int32 RandomIndex = FMath::RandRange(0,Rows.Num()-1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];

			UAssetManager* Manager = UAssetManager::GetIfValid();
			if (Manager)
			{
				LogOnScreen(this,"LoadingMonster",FColor::Green);
				
				TArray<FName> Bundles;
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this,&AARLGameModeBase::OnMonsterLoaded,SelectedRow->MonsterID,Locations[0]);
				Manager->LoadPrimaryAsset(SelectedRow->MonsterID,Bundles,Delegate);
			}
		}
	}
	
}


void AARLGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedID, FVector SpawnLocation)
{
	LogOnScreen(this,"FinishedLoading", FColor::Green);
	
	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		UARLMonsterData* MonsterData =Cast<UARLMonsterData>(Manager->GetPrimaryAssetObject(LoadedID));
		if (MonsterData)
		{
			FVector SpawnLocationFixed = FVector(SpawnLocation.X,SpawnLocation.Y,90);
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocationFixed, FRotator::ZeroRotator);
			if (NewBot)
			{
				LogOnScreen(this, FString::Printf(TEXT("Spawned enemy : %s (%s)"),*GetNameSafe(NewBot),*GetNameSafe(MonsterData)));

				UARLActionComponent* ActionComponent = Cast<UARLActionComponent>(NewBot->GetComponentByClass(UARLActionComponent::StaticClass()));
				if (ActionComponent)
				{
					for (TSubclassOf<UARLAction> ActionClass : MonsterData->Actions)
					{
						ActionComponent->AddAction(NewBot,ActionClass);
					}
				}
			}
		}
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




void AARLGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	AARLPlayerState* PlayerState = NewPlayer->GetPlayerState<AARLPlayerState>();
	if (PlayerState)
	{
		PlayerState->LoadPlayerState(CurrentSaveGame);
	}
	
}

void AARLGameModeBase::WriteSaveGame()
{
	//iterate all player states 
	for (int32 i =0 ; i < GameState->PlayerArray.Num(); i++)
	{
		AARLPlayerState* PlayerState = Cast<AARLPlayerState>(GameState->PlayerArray[i]);
		if (PlayerState)
		{
			PlayerState->SavePlayerState(CurrentSaveGame);
			//break;
		}
	}

	CurrentSaveGame->SavedActors.Empty();
	
	 //iterate all actors
	for (AActor* actor : TActorRange<AActor>(GetWorld()))
	{
		if (actor ->Implements<UARLGameplayInterface>())
		{
			FActorSaveData ActorData;
			ActorData.ActorName = actor->GetName();
			ActorData.Transform = actor->GetActorTransform();

			//pass the array to fill with data from actor 
			FMemoryWriter MemoryWriter(ActorData.ByteData);
			
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter,true);

			//find only variables wiht upropety (savegame)
			Archive.ArIsSaveGame = true;

			//converst actors savegames uproperties to binary array
			actor->Serialize(Archive);
			
			
			CurrentSaveGame->SavedActors.Add(ActorData);
		}
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame,SlotName,0);
}

void AARLGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UARLSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("failed to load savegame data "));
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("loaded savegame data"));

		for (AActor* actor : TActorRange<AActor>(GetWorld()))
		{
			if (actor->Implements<UARLGameplayInterface>())
			{
				for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
				{
					if (ActorData.ActorName == actor->GetName())
					{
						actor->SetActorTransform(ActorData.Transform);

						FMemoryReader MemoryReader(ActorData.ByteData);
						FObjectAndNameAsStringProxyArchive Archive(MemoryReader,true);

						//convert binary array back to actors variables
						actor->Serialize(Archive);
						
						IARLGameplayInterface::Execute_OnActorLoaded(actor);
						
						//UE_LOG(LogTemp, Warning, TEXT("actor loaded : %s"),*actor->GetName());
						break;
					}
				}
			}
		}
		
	}
	else
	{
		CurrentSaveGame = Cast<UARLSaveGame>(UGameplayStatics::CreateSaveGameObject(UARLSaveGame::StaticClass()));

		UE_LOG(LogTemp, Warning, TEXT("created new savegame data"));
	}
	
}



