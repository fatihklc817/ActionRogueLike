// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLItemChest.h"

#include "Net/UnrealNetwork.h"

void AARLItemChest::Interact_Implementation(APawn* InstigatorPawn)
{

	bLidOpened = !bLidOpened;
	OnRep_LidOpened();

}

void AARLItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();
	
}

void AARLItemChest::OnRep_LidOpened()
{
	float CurrentPitch = bLidOpened ? TargetPitch : 0;
	LidMesh->SetRelativeRotation(FRotator(CurrentPitch,0,0));
}

// Sets default values
AARLItemChest::AARLItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("baseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("lidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;

	bReplicates = true;
	//SetReplicates(true);
}

void AARLItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARLItemChest,bLidOpened);
}






