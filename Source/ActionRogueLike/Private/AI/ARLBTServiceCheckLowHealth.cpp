// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLBTServiceCheckLowHealth.h"

#include "AIController.h"
#include "ARLAttributeComponent.h"
#include "AI/ARLAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void UARLBTServiceCheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComponent))
	{
		AAIController* aiController = OwnerComp.GetAIOwner();
		if (ensure(aiController))
		{
			AARLAICharacter* MyPawn = Cast<AARLAICharacter>(aiController->GetPawn()) ;
			if (ensure(MyPawn))
			{
				UARLAttributeComponent* pawnAttributeComp = MyPawn->AttributeComponent;
				if (pawnAttributeComp)
				{
					float MaxHealth;
					float Heatlh = pawnAttributeComp->GetHealthAndMaxHealth(MaxHealth);
					if (Heatlh <= MaxHealth / 3 )
					{
						BlackboardComponent->SetValueAsBool(IsLowHealthKey.SelectedKeyName,true);
					}
					
				}
			}
		}
	}
	

	
}
