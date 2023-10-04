// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLInteractionComponent.h"

#include "ARLGameplayInterface.h"

// Sets default values for this component's properties
UARLInteractionComponent::UARLInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UARLInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UARLInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UARLInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	AActor* MyOwner = GetOwner();
	

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);
	FVector EndPos = EyeLocation + (EyeRotation.Vector() * 1000);
	
	//FHitResult HitResult;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(HitResult,EyeLocation,EndPos,ObjectQueryParams);

	TArray<FHitResult> HitResults;
	float Radius = 30;
	FCollisionShape shape;
	shape.SetSphere(Radius);
	
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(HitResults,EyeLocation,EndPos,FQuat::Identity,ObjectQueryParams,shape);
	
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	
	for (FHitResult HitResult : HitResults)
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			if (HitActor->Implements<UARLGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				IARLGameplayInterface::Execute_Interact(HitActor,MyPawn);
				break;
			}
		
		}
		DrawDebugSphere(GetWorld(),HitResult.ImpactPoint,Radius,16,LineColor,false,2);
	}
	

	
	DrawDebugLine(GetWorld(),EyeLocation,EndPos,LineColor,false,2);
}
