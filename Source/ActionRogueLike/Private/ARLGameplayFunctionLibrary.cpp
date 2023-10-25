// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLGameplayFunctionLibrary.h"

#include "ARLAttributeComponent.h"

bool UARLGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float damageAmount)
{
	UARLAttributeComponent* AttibureComp = UARLAttributeComponent::GetAttributes(TargetActor);
	if (AttibureComp)
	{
		return AttibureComp-> ApplyHealthChange(DamageCauser,-damageAmount);
	}
	return false;
}

bool UARLGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
	const FHitResult& hitResult)
{
	if (ApplyDamage(DamageCauser,TargetActor,DamageAmount))
	{
		UPrimitiveComponent* hittedComp = hitResult.GetComponent();
		if (hittedComp && hittedComp->IsSimulatingPhysics(hitResult.BoneName))
		{
			hittedComp->AddImpulseAtLocation(-hitResult.ImpactNormal * 300000,hitResult.ImpactPoint,hitResult.BoneName);
		}
		return true;
	}
	return false;
}
