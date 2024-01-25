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
	Character->StorageBox = this;

	if (ASCPlayerController* SCPlayerController = IPlayerInterface::Execute_GetSCPlayerController(Character))
	{
		SCPlayerController->ClientToggleStorage(StorageSize, StorageType);

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
}

void ASCStorage::BeginPlay()
{
	Super::BeginPlay();

	const FItemInformation Item = FItemInformation();
	StorageComponent->InitializeItems(Item, StorageSize);
}

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
	
	for (ASCCharacter* AccessingCharacter : AccessingCharacters)
	{
		if (IsValid(AccessingCharacter); ASCPlayerController* AccessingCharacterPC = IPlayerInterface::Execute_GetSCPlayerController(AccessingCharacter))
		{
			AccessingCharacterPC->ClientUpdateStorageSlots(StorageComponent->GetItems().Num());

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
}

void ASCStorage::RemoveAccessingCharacter(ASCCharacter* Character)
{
	AccessingCharacters.Remove(Character);
}
