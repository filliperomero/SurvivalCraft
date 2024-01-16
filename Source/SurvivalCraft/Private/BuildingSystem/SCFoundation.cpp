// Copyright Fillipe Romero

#include "BuildingSystem/SCFoundation.h"

ASCFoundation::ASCFoundation()
{
	PrimaryActorTick.bCanEverTick = false;

	// Add Foundation Tag
	Tags.Add(FName("Foundation"));
}
