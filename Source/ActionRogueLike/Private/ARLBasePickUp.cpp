// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLBasePickUp.h"

// Sets default values
AARLBasePickUp::AARLBasePickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;
}

void AARLBasePickUp::Interact_Implementation(APawn* InstigatorPawn)
{
	DisablePot();
}

void AARLBasePickUp::ReActivatePot()
{
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComp->SetHiddenInGame(false);
}

void AARLBasePickUp::DisablePot()
{
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetHiddenInGame(true);
	GetWorldTimerManager().SetTimer(TimerHandle_PotActivate,this,&AARLBasePickUp::ReActivatePot,PotActivateSecond,false);
}


