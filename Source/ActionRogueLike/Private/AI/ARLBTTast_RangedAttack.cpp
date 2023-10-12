// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLBTTast_RangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UARLBTTast_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AAIController* myAiController = OwnerComp.GetAIOwner();
	if (ensure(myAiController))
	{
		 ACharacter* MyPawn = Cast<ACharacter>(myAiController->GetPawn());
		 if (MyPawn == nullptr)
		 {
			 return EBTNodeResult::Failed;
		 }

		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		 if (TargetActor == nullptr)
		 {
			 return EBTNodeResult::Failed;
		 }

		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = MyPawn;

		AActor* spawnedProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass,MuzzleLocation,MuzzleRotation,Params);

		return (spawnedProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
	}
	
	return EBTNodeResult::Failed;
	
}
