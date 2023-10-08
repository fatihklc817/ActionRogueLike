// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLBT_Service_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UARLBT_Service_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//Check distance beetween ai pawn and target actor

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AActor* targetActor  = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if (targetActor)
		{
			AAIController* myAiController = OwnerComp.GetAIOwner();
			if (ensure(myAiController))
			{
				APawn* AIPawn = myAiController->GetPawn();
				if (ensure(AIPawn))
				{
					float Distance = FVector::Distance(targetActor->GetActorLocation(),AIPawn->GetActorLocation());

					bool bWithinRange = Distance < 2000;
					bool bHasLineOfsight =false;
					if (bWithinRange)
					{
						 bHasLineOfsight = myAiController->LineOfSightTo(targetActor);
					}

					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName,(bWithinRange && bHasLineOfsight));
				}
			}
		}
	}
}
