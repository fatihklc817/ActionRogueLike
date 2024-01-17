// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARLAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*,InsitagorActor, UARLAttributeComponent*,OwningComp, float,newHealth, float,Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged, AActor*,InsitagorActor, UARLAttributeComponent*,OwningComp, float,newHealth, float,Delta);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UARLAttributeComponent : public UActorComponent
{
	GENERATED_BODY()


	
	
protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Attributes" , Replicated)
	float Health{100};

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Attributes", Replicated)
	float MaxHealth{100};

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Attributes")
	float Rage{0};
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Attributes")
	float MaxRage{100};
	//healthmax, stamina,strenth

public:
	
	// Sets default values for this component's properties
	UARLAttributeComponent();

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
	static UARLAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	static bool IsActorAlive(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnRageChanged OnRageChanged;
	
	UFUNCTION(BlueprintCallable,Category="Attiribute")
	bool ApplyHealthChange(AActor* InstigatorActor ,float delta);

	UFUNCTION()
	float GetHealthAndMaxHealth(float& getttedMaxHealth);

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* Instigator);

	UFUNCTION(BlueprintCallable)
	bool ChangeRageValue(AActor* InstigatorActor, float delta);

	UFUNCTION(BlueprintCallable)
	float GetRage();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast,Reliable) //@fixme mark as unrealiable once we moved state our of character
	void MulticastHealthChange(AActor* InstigatorActor, float NewHealth, float Delta);

	bool IsFullHealth();
};
