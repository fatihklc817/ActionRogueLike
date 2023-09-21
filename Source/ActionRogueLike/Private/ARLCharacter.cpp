// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLCharacter.h"

// Sets default values
AARLCharacter::AARLCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AARLCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARLCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AARLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

