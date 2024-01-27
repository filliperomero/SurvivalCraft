// Copyright Fillipe Romero

#include "Inventory/SCStorageContainerComponent.h"
#include "BuildingSystem/Storages/SCStorage.h"

USCStorageContainerComponent::USCStorageContainerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool USCStorageContainerComponent::AddItemToIndex(const FItemInformation& Item, int32 Index)
{
	const bool bIsSuccess = Super::AddItemToIndex(Item, Index);
	
	if (bIsSuccess)
	{
		if (ASCStorage* Storage = Cast<ASCStorage>(GetOwner()))
		{
			Storage->UpdateItemSlotToAccessingCharacters(ContainerType, Index, Item);
		}
	}
	
	return bIsSuccess;
}

bool USCStorageContainerComponent::RemoveItemByIndex(int32 Index)
{
	const bool bIsSuccess = Super::RemoveItemByIndex(Index);
	
	if (bIsSuccess)
	{
		if (ASCStorage* Storage = Cast<ASCStorage>(GetOwner()))
		{
			if (Storage->IsBag())
			{
				Storage->UpdateStorageUI();
			}
			else
			{
				Storage->ResetItemSlotToAccessingCharacters(ContainerType, Index);
			}
		}
	}

	return bIsSuccess;
}

void USCStorageContainerComponent::HandleSlotDrop(USCItemsContainerComponent* FromContainer, int32 FromIndex, int32 ToIndex)
{
	switch (FromContainer->ContainerType)
	{
		case EContainerType::ECT_PlayerInventory:
		case EContainerType::ECT_PlayerHotbar:
			FromContainer->TransferItem(this, FromIndex, ToIndex);
			break;
		case EContainerType::ECT_PlayerStorage:
			TransferItem(this, FromIndex, ToIndex);
			break;
		case EContainerType::ECT_PlayerArmor:
			break;
	}
}
