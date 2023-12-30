// Copyright Fillipe Romero

#include "Inventory/SCItemsContainerComponent.h"
#include "Interfaces/PlayerInterface.h"
#include "Player/SCPlayerController.h"

USCItemsContainerComponent::USCItemsContainerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USCItemsContainerComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void USCItemsContainerComponent::InitializeItems(const FItemInformation& Item, const int32 Size)
{
	if (Size <= 0) return;

	Items.Init(Item, Size);
}

void USCItemsContainerComponent::FindEmptySlot(bool& bSuccess, int32& OutEmptyIndex)
{
	bSuccess = false;
	OutEmptyIndex = -1;
	
	for (int32 i = 0; i < Items.Num(); i++)
	{
		if (Items[i].ItemID == 0)
		{
			OutEmptyIndex = i;
			bSuccess = true;

			break;
		}
	}
}

FItemInformation USCItemsContainerComponent::GetItemByIndex(int32 Index)
{
	if (Items.IsValidIndex(Index)) return Items[Index];

	return FItemInformation();
}

bool USCItemsContainerComponent::RemoveItemByIndex(int32 Index)
{
	if (!Items.IsValidIndex(Index)) return false;
	
	Items[Index] = FItemInformation();

	return true;
}

bool USCItemsContainerComponent::IsSlotEmpty(int32 SlotIndex)
{
	if (Items.IsValidIndex(SlotIndex) && Items[SlotIndex].ItemID == 0)
		return true;

	return false;
}

void USCItemsContainerComponent::UpdateUI(int32 Index, const FItemInformation& Item, bool bShouldResetSlot)
{
	if (bShouldResetSlot == false)
	{
		ASCPlayerController* PC = IPlayerInterface::Execute_GetSCPlayerController(GetOwner());
		
		switch (ContainerType)
		{
			case EContainerType::ECT_PlayerInventory:
			case EContainerType::ECT_PlayerHotbar:
				PC->ClientUpdateItemSlot(ContainerType, Index, Item);
				break;
			case EContainerType::ECT_PlayerStorage:
				break;
			case EContainerType::ECT_PlayerArmor:
				break;
		}
	}
}

// Suppose to be a bool, lets see if we need to fix it in the future
void USCItemsContainerComponent::AddItem(FItemInformation Item)
{
	ServerAddItem(Item);
}

void USCItemsContainerComponent::OnSlotDrop(USCItemsContainerComponent* FromContainer, int32 FromIndex, int32 ToIndex)
{
	HandleSlotDrop(FromContainer, FromIndex, ToIndex);
}

void USCItemsContainerComponent::TransferItem(USCItemsContainerComponent* ToComponent, int32 FromIndex, int32 ToIndex)
{
	if (!IsValid(ToComponent)) return;

	const FItemInformation& ItemToTransfer = GetItemByIndex(FromIndex);

	const bool bIsSuccess = ToComponent->AddItemToIndex(ItemToTransfer, ToIndex);

	if (bIsSuccess) RemoveItemByIndex(FromIndex);
}

bool USCItemsContainerComponent::AddItemToIndex(const FItemInformation& Item, int32 Index)
{
	if (IsSlotEmpty(Index) && Items.IsValidIndex(Index))
	{
		Items[Index] = Item;

		return true;
	}

	return false;
}

bool USCItemsContainerComponent::ContainRequiredItems(TArray<FCraftingItemInfo> RequiredItems)
{
	// For cases where we don't have any required items
	if (RequiredItems.Num() <= 0) return true;

	// We assume the RequiredItems array don't have duplicated items in the list. E.g.: 2 of the same ItemID
	TMap<int32, bool> RequiredItemResult;
	
	for (const FCraftingItemInfo& RequiredItem : RequiredItems)
	{
		int32 RequiredQuantity = RequiredItem.ItemQuantity;
		bool bHasQuantity = false;

		for (const FItemInformation& Item : Items)
		{
			if (Item.ItemID == RequiredItem.ItemID)
			{
				if (Item.ItemQuantity >= RequiredQuantity)
				{
					bHasQuantity = true;
					break;
				}
				
				RequiredQuantity -= Item.ItemQuantity;
			}
		}

		RequiredItemResult.Add(RequiredItem.ItemID, bHasQuantity);
	}

	bool bContainRequiredItems = true;

	for (const auto& Result : RequiredItemResult)
	{
		if (Result.Value == false)
		{
			bContainRequiredItems = false;
			break;
		}
	}

	return bContainRequiredItems;
}

bool USCItemsContainerComponent::HasItemsToStack(const FItemInformation& ItemToCheck)
{
	bool bHasItem = false;

	for (const FItemInformation& Item : Items)
	{
		if (Item.ItemID == ItemToCheck.ItemID && Item.ItemQuantity < Item.StackSize)
		{
			bHasItem = true;
			break;
		}
	}
	
	return bHasItem;
}

void USCItemsContainerComponent::AddRemainingItemQuantity(FItemInformation& Item)
{
	bool bSuccess = false;
	int32 EmptyIndex;
	
	if (Item.ItemQuantity > Item.StackSize)
	{
		int32 TotalItemQuantity = Item.ItemQuantity;
		const int32 MaxStackSize = Item.StackSize;

		bool bShouldFindEmptySlots = true;
		while (bShouldFindEmptySlots)
		{
			FindEmptySlot(bSuccess, EmptyIndex);

			if (bSuccess)
			{
				const int32 CurrentSlotQuantity = TotalItemQuantity >= MaxStackSize ? MaxStackSize : TotalItemQuantity;

				Item.ItemQuantity = CurrentSlotQuantity;
				Items[EmptyIndex] = Item;
				UpdateUI(EmptyIndex, Item, false);
							
				TotalItemQuantity = CurrentSlotQuantity + TotalItemQuantity >= MaxStackSize
	                ? FMath::Clamp(TotalItemQuantity - MaxStackSize, 0, TotalItemQuantity)
	                : 0;

				if (TotalItemQuantity <= 0) bShouldFindEmptySlots = false;
			}
			else
			{
				bShouldFindEmptySlots = false;
			}
		}
	}
	else
	{
		FindEmptySlot(bSuccess, EmptyIndex);

		if (bSuccess) 
		{
			Items[EmptyIndex] = Item;

			UpdateUI(EmptyIndex, Item, false);
		}
	}
}

void USCItemsContainerComponent::ServerAddItem_Implementation(FItemInformation Item)
{
	if (Item.bIsStackable)
	{
		if (HasItemsToStack(Item))
		{
			// UpdateExistingItems
			for (int32 Index = 0; Index < Items.Num(); Index++)
			{
				const auto& LocalItem = Items[Index];

				if (LocalItem.ItemID == Item.ItemID && LocalItem.ItemQuantity < LocalItem.StackSize)
				{
					const int32 MaxStackSize = LocalItem.StackSize;
					const int32 TempSlotQuantity = LocalItem.ItemQuantity;
					const int32 TotalItemQuantity = Item.ItemQuantity;
					int32 CurrentSlotQuantity = LocalItem.ItemQuantity;
					
					CurrentSlotQuantity = CurrentSlotQuantity + TotalItemQuantity > MaxStackSize ? MaxStackSize : CurrentSlotQuantity + TotalItemQuantity;

					Item.ItemQuantity = CurrentSlotQuantity;
					Items[Index] = Item;
					UpdateUI(Index, Item, false);

					Item.ItemQuantity = CurrentSlotQuantity + TotalItemQuantity > MaxStackSize
						? FMath::Clamp(TotalItemQuantity - (MaxStackSize - TempSlotQuantity), 0, TotalItemQuantity)
						: 0;
				}
			}
		}

		if (Item.ItemQuantity > 0)
		{
			AddRemainingItemQuantity(Item);
		}
		else return;
	}
	else
	{
		bool bSuccess = false;
		int32 EmptyIndex;

		FindEmptySlot(bSuccess, EmptyIndex);

		if (bSuccess) 
		{
			Items[EmptyIndex] = Item;

			UpdateUI(EmptyIndex, Item, false);
		}
	}
}

void USCItemsContainerComponent::HandleSlotDrop(USCItemsContainerComponent* FromContainer, int32 FromIndex, int32 ToIndex) {}
