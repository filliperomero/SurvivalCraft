// Copyright Fillipe Romero

#include "Inventory/SCItemsContainerComponent.h"

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

// Suppose to be a bool, lets see if we need to fix it in the future
void USCItemsContainerComponent::AddItem(FItemInformation Item)
{
	ServerAddItem(Item);
}

void USCItemsContainerComponent::ServerAddItem_Implementation(FItemInformation Item)
{
	bool bSuccess;
	int32 EmptyIndex;
	
	FindEmptySlot(bSuccess, EmptyIndex);

	if (bSuccess) 
	{
		Items[EmptyIndex] = Item;

		return;
		// return true
	}

	// return false
}
