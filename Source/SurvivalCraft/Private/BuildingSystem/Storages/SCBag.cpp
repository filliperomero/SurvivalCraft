// Copyright Fillipe Romero

#include "BuildingSystem/Storages/SCBag.h"

void ASCBag::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(DestroySelfTimer, this, &ThisClass::DestroySelfFinished, DestroySelfDelay);
	}
}

void ASCBag::DestroySelfFinished()
{
	DestroyStructure(false);
}
