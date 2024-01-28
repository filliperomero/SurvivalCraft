// Copyright Fillipe Romero

#include "BuildingSystem/Storages/SCStorage.h"
#include "BuildingSystem/Storages/SCBag.h"
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

void ASCStorage::DestroyStructure()
{
	bIsPendingDestruction = true;
	
	if (AccessingCharacters.Num() > 0)
	{
		for (ASCCharacter* AccessingCharacter : AccessingCharacters)
		{
			if (IsValid(AccessingCharacter); ASCPlayerController* AccessingCharacterPC = IPlayerInterface::Execute_GetSCPlayerController(AccessingCharacter))
			{
				AccessingCharacterPC->ClientToggleStorage(0, StorageType, true);
			}
		}
	}

	if (!GetStorageComponent()->IsEmpty() && !IsBag())
	{
		const FTransform SpawnTransform = GetActorTransform();
		
		ASCBag* SpawnedBag = GetWorld()->SpawnActor<ASCBag>(BagClass, SpawnTransform);
		
		SpawnedBag->GetStorageComponent()->Items = GetStorageComponent()->GetAvailableItems();
		SpawnedBag->StorageSize = GetStorageComponent()->GetAvailableItems().Num();
	}
	
	Super::DestroyStructure();
}

void ASCStorage::InteractEvent_Implementation(ASCCharacter* Character)
{
	if (bIsPendingDestruction) return;
	
	ASCPlayerController* SCPlayerController = IPlayerInterface::Execute_GetSCPlayerController(Character);

	const int32 SlotsNumber = IsBag() ? StorageComponent->GetAvailableItems().Num() : StorageSize;
	
	if (AccessingCharacters.Find(Character) != INDEX_NONE)
	{
		// Means we're already accessing it and we're trying to close it.
		SCPlayerController->ClientToggleStorage(SlotsNumber, StorageType, true);
		RemoveAccessingCharacter(Character);
		return;
	}
	
	if (!IsSomeoneAccessing())
	{
		OpenStorage();
	}
	
	Character->StorageBox = this;

	SCPlayerController->ClientToggleStorage(SlotsNumber, StorageType);

	AccessingCharacters.AddUnique(Character);

	TArray<FItemInformation> Items = StorageComponent->GetItems();
	for (int32 Index = 0; Index < Items.Num(); Index++)
	{
		FItemInformation& Item = Items[Index];

		if (Item.ItemID == 0)
		{
			SCPlayerController->ClientResetItemSlot(EContainerType::ECT_PlayerStorage, Index);
		}
		else
		{
			SCPlayerController->ClientUpdateItemSlot(EContainerType::ECT_PlayerStorage, Index, Item);
		}
	}
}

void ASCStorage::BeginPlay()
{
	Super::BeginPlay();

	const FItemInformation Item = FItemInformation();
	StorageComponent->InitializeItems(Item, StorageSize);
}

bool ASCStorage::IsSomeoneAccessing() const
{
	return AccessingCharacters.Num() > 0;
}

void ASCStorage::OpenStorage() {}

void ASCStorage::CloseStorage() {}

void ASCStorage::UpdateItemSlotToAccessingCharacters(EContainerType ContainerType, int32 Index,const FItemInformation& Item)
{
	if (AccessingCharacters.Num() <= 0) return;

	for (ASCCharacter* AccessingCharacter : AccessingCharacters)
	{
		if (IsValid(AccessingCharacter); ASCPlayerController* AccessingCharacterPC = IPlayerInterface::Execute_GetSCPlayerController(AccessingCharacter))
		{
			AccessingCharacterPC->ClientUpdateItemSlot(ContainerType, Index, Item);
		}
	}
}

void ASCStorage::ResetItemSlotToAccessingCharacters(EContainerType ContainerType, int32 Index)
{
	if (AccessingCharacters.Num() <= 0) return;

	for (ASCCharacter* AccessingCharacter : AccessingCharacters)
	{
		if (IsValid(AccessingCharacter); ASCPlayerController* AccessingCharacterPC = IPlayerInterface::Execute_GetSCPlayerController(AccessingCharacter))
		{
			AccessingCharacterPC->ClientResetItemSlot(ContainerType, Index);
		}
	}
}

void ASCStorage::UpdateStorageUI()
{
	if (AccessingCharacters.Num() <= 0) return;

	const int32 SlotsNumber = IsBag() ? StorageComponent->GetAvailableItems().Num() : StorageComponent->GetItems().Num();
	
	for (ASCCharacter* AccessingCharacter : AccessingCharacters)
	{
		if (IsValid(AccessingCharacter); ASCPlayerController* AccessingCharacterPC = IPlayerInterface::Execute_GetSCPlayerController(AccessingCharacter))
		{
			AccessingCharacterPC->ClientUpdateStorageSlots(SlotsNumber);

			TArray<FItemInformation> Items = StorageComponent->GetItems();
			for (int32 Index = 0; Index < Items.Num(); Index++)
			{
				FItemInformation& Item = Items[Index];

				if (Item.ItemID == 0)
				{
					AccessingCharacterPC->ClientResetItemSlot(EContainerType::ECT_PlayerStorage, Index);
				}
				else
				{
					AccessingCharacterPC->ClientUpdateItemSlot(EContainerType::ECT_PlayerStorage, Index, Item);
				}
			}
		}
	}

	if (IsBag() && StorageComponent->IsEmpty()) DestroyStructure();
}

void ASCStorage::RemoveAccessingCharacter(ASCCharacter* Character)
{
	if (bIsPendingDestruction) return;
	
	AccessingCharacters.Remove(Character);

	if (!IsSomeoneAccessing())
	{
		CloseStorage();
	}
}

void ASCStorage::RunAction() {}
