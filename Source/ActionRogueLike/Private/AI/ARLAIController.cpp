// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AARLAIController::BeginPlay()
{
	Super::BeginPlay();
	if (ensureMsgf(BehaviourTree,TEXT("Behaviour trr is null please assing behaviourtree to your ai controller !! ")))
	{
		RunBehaviorTree(BehaviourTree);
	}

	// APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this,0);
	// if (MyPawn)
	// {
	// 	GetBlackboardComponent()->SetValueAsObject("TargetActor",MyPawn);
	// }
}
