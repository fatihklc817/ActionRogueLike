// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLAICharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AARLAICharacter::AARLAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	//PawnSensingComp->SetSensingUpdatesEnabled(true);
}

void AARLAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComp->OnSeePawn.AddDynamic(this,&AARLAICharacter::OnPawnSeen);
}

void AARLAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* aiController = Cast<AAIController>(GetController());
	if (aiController)
	{
		UBlackboardComponent* BlackboardComp = aiController->GetBlackboardComponent();
		BlackboardComp->SetValueAsObject("TargetActor",Pawn);

		DrawDebugString(GetWorld(),GetActorLocation(),"Player Spotted",nullptr,FColor::White,4,true);
	}
}




