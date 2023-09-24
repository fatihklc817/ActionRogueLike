// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARLCharacter.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARLCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere,Category="Attack")
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;

	
	
public:
	// Sets default values for this character's properties
	AARLCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	class UARLInteractionComponent* InteractionComp;

	void MoveForward(float value);

	void MoveRight(float value);

	void PrimaryAttack();

	UFUNCTION()
	void PrimaryAttack_TimeElapsed();

	void PrimaryInteract();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
