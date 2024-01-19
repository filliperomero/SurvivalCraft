// Copyright Fillipe Romero

#include "BuildingSystem/SCBed.h"

ASCBed::ASCBed()
{
	PrimaryActorTick.bCanEverTick = false;

	Tags.Add(FName("AboveFoundation"));
}
