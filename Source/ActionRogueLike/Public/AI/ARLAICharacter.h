// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARLAICharacter.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARLAICharacter : public ACharacter
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere,Category="Components")
	class UARLAttributeComponent* AttributeComponent;
	
protected:

	UPROPERTY()
	class UARLWorldUserWidget* ActiveHealthBarWidget;
	
	UPROPERTY(EditDefaultsOnly, Category="ui")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	
	UPROPERTY(VisibleAnywhere,Category="Effects")
	FName TimeToHitParameter;
	
	UPROPERTY(VisibleAnywhere,Category="Components")
	class UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	class UARLActionComponent* ActionComponent;

public:
	// Sets default values for this character's properties
	AARLAICharacter();

protected:

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwninComp, float newHealth, float delta);

	UFUNCTION()
	void SetTargetActor(AActor* newTarget);

};
