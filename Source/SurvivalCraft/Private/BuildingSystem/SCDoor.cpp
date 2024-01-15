// Copyright Fillipe Romero

#include "BuildingSystem/SCDoor.h"
#include "Net/UnrealNetwork.h"

ASCDoor::ASCDoor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASCDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCDoor, bIsDoorOpen);
}

void ASCDoor::InteractEvent_Implementation(ASCCharacter* Character)
{
	bIsDoorOpen = !bIsDoorOpen;

	RotateDoorTimeline();
}

void ASCDoor::OnRep_IsDoorOpen()
{
	RotateDoorTimeline();
}
