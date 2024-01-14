// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLPlayerController.h"

#include "Blueprint/UserWidget.h"

void AARLPlayerController::TogglePauseMenu()
{
	if (PauseMenuWidgetInstance && PauseMenuWidgetInstance->IsInViewport())
	{
		PauseMenuWidgetInstance->RemoveFromParent();
		PauseMenuWidgetInstance  = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		return;
	}
	
	PauseMenuWidgetInstance = CreateWidget<UUserWidget>(this,PauseMenuWidgetClass);
	if (PauseMenuWidgetInstance)
	{
		PauseMenuWidgetInstance->AddToViewport(100);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
		
	}
	
}

void AARLPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu",IE_Pressed,this,&AARLPlayerController::TogglePauseMenu);
}
