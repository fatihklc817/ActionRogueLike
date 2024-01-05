// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARLWorldUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARLWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite,Category="ui" , meta=(ExposeOnSpawn = true))
	AActor* AttachedActor;

	UPROPERTY(EditAnywhere,Category="ui")
	FVector WorldOffset;

protected:

	UPROPERTY(meta=(BindWidget))
	class USizeBox* ParentSizeBox;
	
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
