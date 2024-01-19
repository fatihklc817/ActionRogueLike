// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLMonsterData.h"

FPrimaryAssetId UARLMonsterData::GetPrimaryAssetId() const
{
	//return Super::GetPrimaryAssetId();

	return FPrimaryAssetId("Monsters",GetFName());
	
}
