// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLBTTaskHealSelf.h"

#include "AIController.h"
#include "ARLAttributeComponent.h"
#include "AI/ARLAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UARLBTTaskHealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIController* myAiController = OwnerComp.GetAIOwner();
	if (ensure(myAiController))
	{
		AARLAICharacter* myPawn = Cast<AARLAICharacter>(myAiController->GetPawn());
		if (ensure(myPawn))
		{
			UARLAttributeComponent* pawnsAttributeComp = myPawn -> AttributeComponent;
			if (pawnsAttributeComp)
			{
				float MaxHealth;
				float health = pawnsAttributeComp->GetHealthAndMaxHealth(MaxHealth);
				pawnsAttributeComp->ApplyHealthChange(MaxHealth);
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsLowHealthKey.SelectedKeyName,false);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
