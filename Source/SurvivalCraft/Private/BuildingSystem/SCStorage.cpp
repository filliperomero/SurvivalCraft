// Copyright Fillipe Romero

#include "BuildingSystem/SCStorage.h"
#include "Character/SCCharacter.h"
#include "Interfaces/PlayerInterface.h"
#include "Inventory/SCStorageContainerComponent.h"
#include "Player/SCPlayerController.h"

ASCStorage::ASCStorage()
{
	PrimaryActorTick.bCanEverTick = false;

	StorageComponent = CreateDefaultSubobject<USCStorageContainerComponent>(TEXT("StorageComponent"));
	StorageComponent->ContainerType = EContainerType::ECT_PlayerStorage;
}

void ASCStorage::InteractEvent_Implementation(ASCCharacter* Character)
{
	if (ASCPlayerController* SCPlayerController = IPlayerInterface::Execute_GetSCPlayerController(Character))
	{
		SCPlayerController->ClientToggleStorage(StorageSize, StorageType);
	}
}

void ASCStorage::BeginPlay()
{
	Super::BeginPlay();

	const FItemInformation Item = FItemInformation();
	StorageComponent->InitializeItems(Item, StorageSize);
}
