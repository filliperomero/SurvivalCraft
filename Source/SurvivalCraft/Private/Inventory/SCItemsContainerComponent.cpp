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
				PC->UpdateItemSlot(ContainerType, Index, Item);
				break;
			case EContainerType::ECT_PlayerHotbar:
				PC->UpdateItemSlot(ContainerType, Index, Item);
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

void USCItemsContainerComponent::HandleSlotDrop(USCItemsContainerComponent* FromContainer, int32 FromIndex, int32 ToIndex)
{
}

void USCItemsContainerComponent::ServerAddItem_Implementation(FItemInformation Item)
{
	bool bSuccess;
	int32 EmptyIndex;
	
	FindEmptySlot(bSuccess, EmptyIndex);

	if (bSuccess) 
	{
		Items[EmptyIndex] = Item;

		UpdateUI(EmptyIndex, Item, false);

		return;
		// return true
	}

	// return false
}
