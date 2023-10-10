// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARLAICharacter.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARLAICharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(VisibleAnywhere,Category="Components")
	class UPawnSensingComponent* PawnSensingComp;

public:
	// Sets default values for this character's properties
	AARLAICharacter();

protected:

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);



};
