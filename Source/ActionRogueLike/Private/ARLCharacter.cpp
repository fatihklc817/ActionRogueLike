// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLCharacter.h"

#include "ARLAttributeComponent.h"
#include "ARLBlackHoleProjectile.h"
#include "ARLInteractionComponent.h"
#include "ARLMagicProjectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
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

	GetCharacterMovement()->bOrientRotationToMovement = true;
		
	bUseControllerRotationYaw = false;
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
	PlayAnimMontage(PrimaryAttackAnim);

	//line trace from camera
	FVector PrimaryAttackEndPos;
	RayFromCamera(PrimaryAttackEndPos);

	TimerDelegate_PrimaryAttack.BindUFunction(this,FName("PrimaryAttack_TimeElapsed"),PrimaryAttackEndPos);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack,TimerDelegate_PrimaryAttack,0.2,false);
	
	
}

void AARLCharacter::PrimaryAttack_TimeElapsed(const FVector& EndPos)
{
	FVector HandMuzzleSocketLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	
	SpawnProjectile(EndPos,HandMuzzleSocketLocation,PrimaryMagicProjectileClass);
	
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
	FVector EndPos;
	bool bSuccess = RayFromCamera(EndPos);
	FVector HandMuzzleSocketLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	SpawnProjectile(EndPos , HandMuzzleSocketLocation,BlackHoleProjectileClass);
	
}

void AARLCharacter::SkillE()
{
	//animaation puf
	FVector EndPos;
	bool bSuccess = RayFromCamera(EndPos);

	TimerDelegate_SkillE.BindUFunction(this,FName("SkillE_TimeElapsed"),EndPos);
	GetWorldTimerManager().SetTimer(TimerHandle_SKillE,TimerDelegate_SkillE,0.2,false);
	
}

void AARLCharacter::SkillE_TimeElapsed(const FVector& EndPos)
{
	FVector HandleMuzzleSockerLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	AActor* SpawnedActor = SpawnProjectile(EndPos , HandleMuzzleSockerLocation, TeleportProjectileClass);
	FTimerHandle TimerHandleSkillE_Arrive;
	FTimerDelegate TimerdelegateSkillEArrived;
	TimerdelegateSkillEArrived.BindUFunction(this,FName("SkillE_Arrived"),SpawnedActor);
	GetWorldTimerManager().SetTimer(TimerHandleSkillE_Arrive,TimerdelegateSkillEArrived,0.4,false);
}

void AARLCharacter::SkillE_Arrived(AActor* SpawnedActor)
{
	FVector teleportLocation = SpawnedActor->GetActorLocation();
	SpawnedActor->Destroy();
	//patlama particle
	FTimerHandle timerHandleTeleportPawn;
	GetWorldTimerManager().SetTimer(timerHandleTeleportPawn,[teleportLocation,SpawnedActor]
	{
		AActor* pawn = SpawnedActor->GetInstigator();
		pawn->TeleportTo(teleportLocation,pawn->GetActorRotation());
	},0.2,false); 
	
}

bool AARLCharacter::RayFromCamera(FVector& EndTrace)
{
	FHitResult HitResult;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	FVector StartTrace = CameraComp->GetComponentLocation();
	EndTrace = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() *5000);

	bool bTraceSuccess =  GetWorld()->LineTraceSingleByObjectType(HitResult,StartTrace,EndTrace,ObjectQueryParams);
	//DrawDebugLine(GetWorld(),StartTrace,EndTrace,FColor::Red,false,3,0,3);
	if (bTraceSuccess)
	{
		EndTrace = HitResult.ImpactPoint;
	}
	return bTraceSuccess;
}

AActor* AARLCharacter::SpawnProjectile(FVector Endpos, FVector HandPos, TSubclassOf<AActor> spawnClass)
{
	FVector rotation = Endpos  - HandPos;
	rotation.Normalize();
	FTransform SpawnTransform = FTransform(rotation.Rotation(),HandPos);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	SpawnParameters.Instigator = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(spawnClass, SpawnTransform, SpawnParameters);
	return SpawnedActor;
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
	

}

