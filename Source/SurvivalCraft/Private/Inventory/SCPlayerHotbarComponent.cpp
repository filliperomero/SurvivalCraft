// Copyright Fillipe Romero

#include "Inventory/SCPlayerHotbarComponent.h"

USCPlayerHotbarComponent::USCPlayerHotbarComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool USCPlayerHotbarComponent::HasItemInSlot(const int32 Index, EItemType& ItemType)
{
	if (!Items.IsValidIndex(Index) || Items[Index].ItemID == 0) return false;

	ItemType = Items[Index].ItemType;
	
	return true;
}

void USCPlayerHotbarComponent::BeginPlay()
{
	Super::BeginPlay();
}
