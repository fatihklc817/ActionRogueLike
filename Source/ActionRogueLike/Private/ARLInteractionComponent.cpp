// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLInteractionComponent.h"

#include "ARLGameplayInterface.h"
#include "Blueprint/UserWidget.h"
#include "ARLWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("arlu.InteractionDebugDraw"),false,TEXT("Enable debug lines for interact component"),ECVF_Cheat);

UARLInteractionComponent::UARLInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TraceRadius = 30;
	TraceDistance = 500;
	CollisionChannel = ECC_WorldDynamic;
}


void UARLInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UARLInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void UARLInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

void UARLInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("no focus actor to interact"));
		return;
	}
	
	APawn* MyPawn = Cast<APawn>(GetOwner());
	IARLGameplayInterface::Execute_Interact(InFocus,MyPawn);
	
}

void UARLInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	AActor* MyOwner = GetOwner();
	

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);
	FVector EndPos = EyeLocation + (EyeRotation.Vector() * TraceDistance);
	
	//FHitResult HitResult;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(HitResult,EyeLocation,EndPos,ObjectQueryParams);

	TArray<FHitResult> HitResults;
	FCollisionShape shape;
	shape.SetSphere(TraceRadius);
	
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(HitResults,EyeLocation,EndPos,FQuat::Identity,ObjectQueryParams,shape);
	
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	FocusedActor = nullptr;  //clear ref before trying to find
	
	for (FHitResult HitResult : HitResults)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(),HitResult.ImpactPoint,TraceRadius,16,LineColor,false,2);
			
		}
		
		if (AActor* HitActor = HitResult.GetActor())
		{
			if (HitActor->Implements<UARLGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		
		}
		
	}

	if (FocusedActor)
	{
		if (!DefaultWidgetInstance && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<UARLWorldUserWidget>(GetWorld(),DefaultWidgetClass);  
		}
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			
			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}


	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(),EyeLocation,EndPos,LineColor,false,2);
	}
}
