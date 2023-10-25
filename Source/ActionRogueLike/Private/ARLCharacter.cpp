// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLCharacter.h"

#include "ARLActionComponent.h"
#include "ARLAttributeComponent.h"
#include "ARLBlackHoleProjectile.h"
#include "ARLInteractionComponent.h"
#include "ARLMagicProjectile.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GeometryCollection/GeometryCollectionSimulationTypes.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AARLCharacter::AARLCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UARLInteractionComponent>("InteractionComp");

	AttributeComponent = CreateDefaultSubobject<UARLAttributeComponent>("AttributeComp");

	ActionComponent = CreateDefaultSubobject<UARLActionComponent>("ActionComp");


	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;
	
}

void AARLCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnHealthChanged.AddDynamic(this,&AARLCharacter::onHealthChanged);
}

// Called when the game starts or when spawned
void AARLCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AARLCharacter::MoveForward(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;
	
	AddMovementInput(ControlRot.Vector(),value);
}

void AARLCharacter::MoveRight(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;

	//x forward red // y right green // z up blue
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);  // getting rotator's y axis 
	
	AddMovementInput(RightVector,value);
}

void AARLCharacter::PrimaryAttack()
{
	ActionComponent->StartActionByName(this,"PrimaryAttack");
	
}


void AARLCharacter::PrimaryInteract()
{
	if (ensureAlways(InteractionComp))
	{
		InteractionComp->PrimaryInteract();
	}
}

void AARLCharacter::BlackholeSkill()
{
	ActionComponent->StartActionByName(this,"BlackHoleAttack");
}


void AARLCharacter::SkillE()
{
	ActionComponent->StartActionByName(this,"Dash");
}


void AARLCharacter::SprintStart()
{
	ActionComponent->StartActionByName(this,"Sprint");
}

void AARLCharacter::SprintStop()
{
	ActionComponent->StopActionByName(this,"Sprint");
}



void AARLCharacter::onHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwninComp, float newHealth, float delta)
{
	if (delta < 0 )
	{
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit",GetWorld()->TimeSeconds);
		FVector MyColorVector = FVector(hitFlashColor.R,hitFlashColor.G,hitFlashColor.B);
		GetMesh()->SetVectorParameterValueOnMaterials("HitFlashColor",MyColorVector);
	}
	
	if (newHealth <= 0 && delta < 0)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		  
	}
}

FVector AARLCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}


// Called every frame
void AARLCharacter::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AARLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",this,&AARLCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&AARLCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn",this,&APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp",this,&APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&AARLCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryAttack",IE_Pressed,this,&AARLCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract",IE_Pressed,this,&AARLCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("SkillQ",IE_Pressed,this,&AARLCharacter::BlackholeSkill);
	PlayerInputComponent->BindAction("SkillE",IE_Pressed,this,&AARLCharacter::SkillE);

	PlayerInputComponent->BindAction("Sprint",IE_Pressed,this,&AARLCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint",IE_Released,this,&AARLCharacter::SprintStop);
	

}

void AARLCharacter::HealSelf(float amount /* =100 */)
{
	AttributeComponent->ApplyHealthChange(this,amount);
}

