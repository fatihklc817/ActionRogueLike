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
	UPROPERTY(EditAnywhere)
	FLinearColor hitFlashColor;
	
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	class UARLInteractionComponent* InteractionComp;
	
public:
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly , Category="Components")
	class UARLAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Components")
	class UARLActionComponent* ActionComponent;

	
	
public:
	
	AARLCharacter();

protected:

	virtual void PostInitializeComponents() override;
	
	
	virtual void BeginPlay() override;


	void MoveForward(float value);

	void MoveRight(float value);

	void PrimaryAttack();
	
	UFUNCTION()
	void PrimaryInteract();

	UFUNCTION()
	void BlackholeSkill();
	
	UFUNCTION()
	void SkillE();
	
	UFUNCTION()
	void SprintStart();

	UFUNCTION()
	void SprintStop();
	
	UFUNCTION()
	void onHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwninComp, float newHealth, float delta);

	UFUNCTION()
	virtual FVector GetPawnViewLocation() const override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float amount = 100);

};

