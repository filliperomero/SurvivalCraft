// Copyright Fillipe Romero

#include "BuildingSystem/Storages/SCStorageCrate.h"
#include "Net/UnrealNetwork.h"

ASCStorageCrate::ASCStorageCrate()
{
	PrimaryActorTick.bCanEverTick = false;

	StorageLidMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StorageLidMeshComponent"));
	StorageLidMeshComponent->SetupAttachment(GetMesh());
	StorageLidMeshComponent->SetRelativeLocation(FVector(0.f, -35.f, 52.f));
}

void ASCStorageCrate::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCStorageCrate, bIsLidOpen);
}

void ASCStorageCrate::OnRep_IsLidOpen()
{
	RotateLidTimeline();
}

void ASCStorageCrate::OpenStorage()
{
	bIsLidOpen = true;
	RotateLidTimeline();
}

void ASCStorageCrate::CloseStorage()
{
	bIsLidOpen = false;
	RotateLidTimeline();
}
