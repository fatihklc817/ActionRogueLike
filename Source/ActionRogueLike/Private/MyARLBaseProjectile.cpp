// Fill out your copyright notice in the Description page of Project Settings.


#include "MyARLBaseProjectile.h"

void AMyARLBaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("mynewClasslog"));
}
