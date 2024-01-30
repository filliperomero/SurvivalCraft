// Copyright Fillipe Romero

#include "Inventory/SCItemsContainerComponent.h"
#include "BuildingSystem/Storages/SCStorage.h"
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

bool USCItemsContainerComponent::IsEmpty()
{
	if (Items.Num() <= 0) return true;

	bool bIsEmpty = true;

	for (const auto& Item : Items)
	{
		if (Item.ItemID != 0)
		{
			bIsEmpty = false;
			break;
		}
	}

	return bIsEmpty;
}

bool USCItemsContainerComponent::IsFull()
{
	if (Items.Num() <= 0) return true;

	bool bIsFull = true;

	for (const auto& Item : Items)
	{
		if (Item.ItemID == 0)
		{
			bIsFull = false;
			break;
		}
	}

	return bIsFull;
}

TArray<FItemInformation> USCItemsContainerComponent::GetAvailableItems()
{
	TArray<FItemInformation> AvailableItems;
	
	for (auto Item : Items)
	{
		if (Item.ItemID != 0)
		{
			AvailableItems.Add(Item);
		}	
	}

	return AvailableItems;
}

void USCItemsContainerComponent::DropItem(int32 Index) {}

void USCItemsContainerComponent::SplitItemStack(int32 Index)
{
	if (IsSlotEmpty(Index) || IsFull()) return;

	FItemInformation ItemInformation = GetItems()[Index];

	const int32 CurrentQuantity = ItemInformation.ItemQuantity;

	if (CurrentQuantity >= 2)
	{
		const int32 SplitQuantity = CurrentQuantity / 2;

		UpdateItemQuantity(Index, SplitQuantity);
		ItemInformation.ItemQuantity = CurrentQuantity - SplitQuantity;
		AddItem(ItemInformation, true);
	}
}

bool USCItemsContainerComponent::IsSlotEmpty(int32 SlotIndex)
{
	if (Items.IsValidIndex(SlotIndex) && Items[SlotIndex].ItemID == 0)
		return true;

	return false;
}

void USCItemsContainerComponent::UpdateUI(int32 Index, const FItemInformation& Item, bool bShouldResetSlot)
{
	switch (ContainerType)
	{
		case EContainerType::ECT_PlayerInventory:
		case EContainerType::ECT_PlayerHotbar:
			{
				ASCPlayerController* PC = IPlayerInterface::Execute_GetSCPlayerController(GetOwner());
			
				if (bShouldResetSlot) PC->ClientResetItemSlot(ContainerType, Index);
				else PC->ClientUpdateItemSlot(ContainerType, Index, Item);

				break;
			}
		case EContainerType::ECT_PlayerStorage:
			{
				if (ASCStorage* Storage = Cast<ASCStorage>(GetOwner()))
				{
					Storage->UpdateItemSlotToAccessingCharacters(EContainerType::ECT_PlayerStorage, Index, Item);
				}
			
				break;
			}
		case EContainerType::ECT_PlayerArmor:
			break;
	}
}

// Suppose to be a bool, lets see if we need to fix it in the future
void USCItemsContainerComponent::AddItem(FItemInformation Item, bool bIsItemSplit)
{
	ServerAddItem(Item, bIsItemSplit);
}

void USCItemsContainerComponent::OnSlotDrop(USCItemsContainerComponent* FromContainer, int32 FromIndex, int32 ToIndex)
{
	HandleSlotDrop(FromContainer, FromIndex, ToIndex);
}

void USCItemsContainerComponent::TransferItem(USCItemsContainerComponent* ToComponent, int32 FromIndex, int32 ToIndex)
{
	if (!IsValid(ToComponent) || FromIndex == ToIndex) return;

	const FItemInformation& ItemToTransfer = GetItemByIndex(FromIndex);

	int32 OutRemainingQuantity = -1;

	const bool bIsSuccess = ToComponent->AddItemToIndex(ItemToTransfer, FromIndex, ToIndex, OutRemainingQuantity);

	if (OutRemainingQuantity > 0)
	{
		UpdateItemQuantity(FromIndex, OutRemainingQuantity);
	}

	if (bIsSuccess && OutRemainingQuantity <= 0) RemoveItemByIndex(FromIndex);
}

bool USCItemsContainerComponent::AddItemToIndex(const FItemInformation& Item, int32 FromIndex, int32 ToIndex, int32& OutRemainingQuantity)
{
	if (!Items.IsValidIndex(ToIndex)) return false;
	
	if (IsSlotEmpty(ToIndex))
	{
		Items[ToIndex] = Item;

		// -1 Indicates we don't have a Stack/Swap
		OutRemainingQuantity = -1;

		return true;
	}

	return StackItem(Item, ToIndex, OutRemainingQuantity);
}

bool USCItemsContainerComponent::StackItem(const FItemInformation& Item, int32 ToIndex, int32& OutRemainingQuantity)
{
	if (!Items.IsValidIndex(ToIndex)) return false;

	const FItemInformation TargetItem = GetItems()[ToIndex];

	// Here we can Swap Items if we want
	if (Item.ItemID != TargetItem.ItemID || !Item.bIsStackable || !TargetItem.bIsStackable) return false;

	if (TargetItem.ItemQuantity >= TargetItem.StackSize) return false;

	const int32 CombinedQuantity = TargetItem.ItemQuantity + Item.ItemQuantity;
	const int32 TargetStackSize = TargetItem.StackSize;

	if (CombinedQuantity > TargetStackSize)
	{
		UpdateItemQuantity(ToIndex, TargetStackSize);
		OutRemainingQuantity = CombinedQuantity - TargetStackSize;
		
		return true;
	}

	UpdateItemQuantity(ToIndex, CombinedQuantity);
	OutRemainingQuantity = 0;

	return true;
}

bool USCItemsContainerComponent::RemoveItems(TArray<FCraftingItemInfo> ItemsToRemove)
{
	if (ItemsToRemove.Num() <= 0) return true;

	bool bAbortRemoveItems = false;

	TMap<int32/*Index in array*/, int32/*QuantityToRemove*/> ItemsToRemoveMap;

	for (const FCraftingItemInfo& ItemToRemove : ItemsToRemove)
	{
		int32 QuantityToRemove = ItemToRemove.ItemQuantity;
		
		for (int32 Index = 0; Index < Items.Num(); Index++)
		{
			const FItemInformation& Item = Items[Index];
			
			if (Item.ItemID == ItemToRemove.ItemID)
			{
				if (Item.ItemQuantity >= QuantityToRemove)
				{
					ItemsToRemoveMap.Add(Index, QuantityToRemove);
					QuantityToRemove = 0;
					break;
				}
				
				ItemsToRemoveMap.Add(Index, Item.ItemQuantity);
				QuantityToRemove -= Item.ItemQuantity;
			}
		}

		if (QuantityToRemove > 0)
		{
			bAbortRemoveItems = true;
			break;
		}
	}

	// If we're aborting, we have a bigger problem because we should be calling ContainRequiredItems before calling this function
	if (bAbortRemoveItems) return false;

	// Delete all necessary items
	for (const auto& ItemToRemove : ItemsToRemoveMap)
	{
		const int32 SlotIndex = ItemToRemove.Key;
		const int32 ItemQuantityToRemove = ItemToRemove.Value;
		
		if (Items[SlotIndex].ItemQuantity == ItemQuantityToRemove)
		{
			// Note for the future: If RemoveItemByIndex is not updating the UI, remember that updating the UI is done in the child's class of the RemoveItemByIndex
			RemoveItemByIndex(SlotIndex);
		}
		else
		{
			Items[SlotIndex].ItemQuantity = Items[SlotIndex].ItemQuantity - ItemQuantityToRemove;
			UpdateUI(SlotIndex, Items[SlotIndex], false);
		}
	}
	
	return true;
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

bool USCItemsContainerComponent::RemoveItemQuantity(int32 Index, int32 AmountToRemove)
{
	if (Items.IsValidIndex(Index))
	{
		const int32 NewAmount = Items[Index].ItemQuantity - AmountToRemove;
		
		return UpdateItemQuantity(Index, NewAmount);
	}

	return false;
}

bool USCItemsContainerComponent::UpdateItemQuantity(int32 Index, int32 NewQuantity)
{
	if (Items.IsValidIndex(Index))
	{
		if (NewQuantity <= 0)
		{
			return RemoveItemByIndex(Index);
		}
		else
		{
			Items[Index].ItemQuantity = NewQuantity;

			UpdateUI(Index, Items[Index], false);

			return true;
		}
	}

	return false;
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

void USCItemsContainerComponent::ServerAddItem_Implementation(FItemInformation Item, bool bIsItemSplit)
{
	if (Item.bIsStackable && !bIsItemSplit)
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
