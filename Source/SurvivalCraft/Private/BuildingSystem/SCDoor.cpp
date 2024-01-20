// Copyright Fillipe Romero

#include "BuildingSystem/SCDoor.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

ASCDoor::ASCDoor()
{
	PrimaryActorTick.bCanEverTick = false;

	TextOverlapSphere->SetRelativeLocation(FVector(50.f, 0.f, 100.f));

	InfoWidget->SetupAttachment(GetMesh());
	InfoWidget->SetRelativeLocation(FVector(50.f, 0.f, 100.f));
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
