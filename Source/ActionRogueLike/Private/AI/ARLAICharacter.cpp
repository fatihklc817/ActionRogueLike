// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLAICharacter.h"

#include "AIController.h"
#include "ARLActionComponent.h"
#include "ARLAttributeComponent.h"
#include "ARLWorldUserWidget.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AARLAICharacter::AARLAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComponent = CreateDefaultSubobject<UARLAttributeComponent>("AttributeComp");
	//PawnSensingComp->SetSensingUpdatesEnabled(true);

	ActionComponent = CreateDefaultSubobject<UARLActionComponent>("ActionComp");
	
	GetMesh()->SetGenerateOverlapEvents(true);

	TimeToHitParameter = "TimeToHit";
}

void AARLAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComp->OnSeePawn.AddDynamic(this,&AARLAICharacter::OnPawnSeen);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &AARLAICharacter::OnHealthChanged);
}

void AARLAICharacter::OnPawnSeen(APawn* Pawn)
{
		SetTargetActor(Pawn);
		MulticastOnPawnSeen();
}

void AARLAICharacter::MulticastOnPawnSeen_Implementation()
{
	DrawDebugString(GetWorld(),GetActorLocation(),"Player Spotted",nullptr,FColor::White,4,true);
	if (SpottedWidget == nullptr)
	{
		SpottedWidget = CreateWidget<UARLWorldUserWidget>(GetWorld(),SpottedWidgetClass);
		if (SpottedWidget)
		{
			SpottedWidget->AttachedActor = this;
			SpottedWidget->AddToViewport();
		}
	}
	else
	{
		if (!SpottedWidget->IsInViewport())
		{
			SpottedWidget->AddToViewport();
		}
	}
}

void AARLAICharacter::OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwninComp, float newHealth, float delta)
{
	if (delta < 0)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if (ActiveHealthBarWidget == nullptr)
		{
			ActiveHealthBarWidget = CreateWidget<UARLWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBarWidget)
			{
				ActiveHealthBarWidget->AttachedActor = this;
				//ActiveHealthBarWidget->WorldOffset = FVector(0,0,30);  //I sette world offset from bp
				ActiveHealthBarWidget->AddToViewport();
			}
		}
	
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParameter,GetWorld()->TimeSeconds);
		
		if (newHealth <= 0)
		{
			//stop bt
			AAIController* AiController = Cast<AAIController>(GetController());
			if (AiController)
			{
				AiController->GetBrainComponent()->StopLogic("Killed");
			}
			
			//ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			
			//set lifespawn
			SetLifeSpan(10);
		}
	}
}

void AARLAICharacter::SetTargetActor(AActor* newTarget)
{
	AAIController* aiController = Cast<AAIController>(GetController());
	if (aiController)
	{
		aiController->GetBlackboardComponent()->SetValueAsObject("TargetActor",newTarget);
	}
}




