// Copyright Fillipe Romero

#include "Inventory/SCStorageContainerComponent.h"
#include "BuildingSystem/Storages/SCStorage.h"
#include "Items/SCWorldItemBag.h"

USCStorageContainerComponent::USCStorageContainerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool USCStorageContainerComponent::AddItemToIndex(const FItemInformation& Item, int32 FromIndex, int32 ToIndex, int32& OutRemainingQuantity)
{
	const bool bIsSuccess = Super::AddItemToIndex(Item, FromIndex, ToIndex, OutRemainingQuantity);

	if (bIsSuccess && OutRemainingQuantity == -1)
	{
		if (ASCStorage* Storage = Cast<ASCStorage>(GetOwner()))
		{
			Storage->UpdateItemSlotToAccessingCharacters(ContainerType, ToIndex, Item);
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

void USCStorageContainerComponent::DropItem(int32 Index)
{
	Super::DropItem(Index);
	checkf(WorldItemBagClass, TEXT("WorldItemBagClass not set. Please fill out WorldItemBagClass property"));

	if (IsSlotEmpty(Index)) return;

	if (const ASCStorage* Storage = Cast<ASCStorage>(GetOwner()))
	{
		FTransform Transform;
		Transform.SetLocation(Storage->GetActorLocation() + Storage->GetDropBagOffset());
		
		ASCWorldItemBag* SpawnedWorldItemBag = GetWorld()->SpawnActorDeferred<ASCWorldItemBag>(WorldItemBagClass, Transform);
		
		SpawnedWorldItemBag->SetBagItemInfo(GetItems()[Index]);
		SpawnedWorldItemBag->FinishSpawning(Transform);

		RemoveItemByIndex(Index);
	}
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
