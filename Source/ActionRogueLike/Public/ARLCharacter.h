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

	UPROPERTY(EditAnywhere,Category="Attack")
	TSubclassOf<AActor> BlackHoleProjectileClass;
	
	UPROPERTY(EditAnywhere,Category="Attack")
	TSubclassOf<AActor> PrimaryMagicProjectileClass;

	UPROPERTY(EditAnywhere,Category="Attack")
	TSubclassOf<AActor> TeleportProjectileClass;
	

	UPROPERTY(EditAnywhere,Category="Attack")
	UAnimMontage* PrimaryAttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;

	FTimerDelegate TimerDelegate_PrimaryAttack;

	FTimerHandle TimerHandle_SKillE;
	FTimerDelegate TimerDelegate_SkillE;

	
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	class UARLInteractionComponent* InteractionComp;
	
public:
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly , Category="Components")
	class UARLAttributeComponent* AttributeComponent;

	
	
public:
	// Sets default values for this character's properties
	AARLCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	void MoveForward(float value);

	void MoveRight(float value);

	void PrimaryAttack();

	UFUNCTION()
	void PrimaryAttack_TimeElapsed(const FVector& EndPos);

	UFUNCTION()
	void PrimaryInteract();

	UFUNCTION()
	void BlackholeSkill();

	UFUNCTION()
	void SkillE();

	UFUNCTION()
	void SkillE_TimeElapsed(const FVector& EndPos);

	UFUNCTION()
	void SkillE_Arrived(AActor* SpawnedActor);

	UFUNCTION()
	bool RayFromCamera(FVector &EndTrace);

	UFUNCTION()
	AActor* SpawnProjectile(FVector Endpos, FVector HandPos , TSubclassOf<AActor> spawnClass);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
