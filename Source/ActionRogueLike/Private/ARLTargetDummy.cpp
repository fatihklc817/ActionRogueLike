// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLTargetDummy.h"

#include "ARLAttributeComponent.h"

// Sets default values
AARLTargetDummy::AARLTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComponent;

	AttributeComponent = CreateDefaultSubobject<UARLAttributeComponent>("AttributeComp");
	AttributeComponent->OnHealthChanged.AddDynamic(this,&AARLTargetDummy::onHealthChanged);

}

// Called when the game starts or when spawned
void AARLTargetDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AARLTargetDummy::onHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwninComp, float newHealth, float delta)
{
	if (delta < 0)
	{
		MeshComponent->SetScalarParameterValueOnMaterials("TimeToHit",GetWorld()->TimeSeconds);
	}
}

// Called every frame
void AARLTargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

