// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLBasePickUp.h"

#include "Net/UnrealNetwork.h"


// Sets default values
AARLBasePickUp::AARLBasePickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	SetReplicates(true);

	bIsActive = true;
}

void AARLBasePickUp::Interact_Implementation(APawn* InstigatorPawn)
{
	DisablePickup();
}

FText AARLBasePickUp::GetInteractionText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

void AARLBasePickUp::ReActivatePickup()
{
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComp->SetHiddenInGame(false);
	bIsActive = true;
}

void AARLBasePickUp::DisablePickup()
{
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetHiddenInGame(true);
	GetWorldTimerManager().SetTimer(TimerHandle_PickupActivate,this,&AARLBasePickUp::ReActivatePickup,PickupActivateSecond,false);
	bIsActive = false;
}

void AARLBasePickUp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARLBasePickUp,bIsActive);
}


void AARLBasePickUp::OnRep_IsActive()
{
	if (bIsActive)
	{
		ReActivatePickup();
	}
	else
	{
		DisablePickup();
	}
}
